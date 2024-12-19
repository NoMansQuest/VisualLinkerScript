#include "CMemoryRegionBlock.h"
#include "COverlayStatement.h"
#include "CSectionStatement.h"
#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double distanceFromFirstSectionToRegionTopInMmm = 3; // 3mm margin

void CMemoryRegionBlock::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	// Draw header & size
	painter.setFont(graphicContext.FontLarge());
	painter.setPen(QPen(QColor::fromRgba(Colors::MemoryRegionDefaultForeColor)));

	painter.drawText(
		this->TitleArea().ConvertToQRect(graphicContext),
		Qt::AlignHCenter | Qt::AlignVCenter,
		QString::fromStdString(this->Title()));

	painter.setFont(graphicContext.FontLargeBold());
	painter.drawText(
		this->MemorySizeTextArea().ConvertToQRect(graphicContext),
		Qt::AlignHCenter | Qt::AlignVCenter,
		QString::fromStdString(this->MemorySizeText()));


	const auto borderPen = QPen(QColor::fromRgba(Colors::MemoryRegionDefaultBorderColor), 2, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(Colors::MemoryRegionDefaultBackgroundColor), Qt::SolidPattern);
	this->PaintAddressedRegion(graphicContext, painter, borderPen, fillBrush);

	// Draw all children
	for (const auto& childContent : this->ChildContent())
	{
		childContent->Paint(graphicContext, painter);
	}
}

SMetricSizeF CMemoryRegionBlock::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	// Top memory label
	auto topMemoryLabelAndSize = Graphical::GetTextWidthInPixels(this->Title() + this->MemorySizeText(), graphicContext.FontMetricsLarge());

	double calculatedWidth = 0;
	double calculatedHeight = (graphicContext.FontMetricsLarge().height() * 1.5) + (memoryRegionBorderMm * 2); // Initial height

	if (!this->m_ChildContent.empty())
	{
		calculatedHeight += distanceFromFirstSectionToRegionTopInMmm;

		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			calculatedWidth = std::max(calculatedWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + sectionToSectionVSpaceMm;
		}
	}

	return { std::max(topMemoryLabelAndSize, calculatedWidth), calculatedHeight };
}

void CMemoryRegionBlock::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
	auto currentYHolder = allocatedArea.Top();

	auto topMemoryLabelRect =
		SMetricRectangleF(graphicContext.FontMetricsLarge().size(Qt::TextSingleLine,QString::fromStdString(this->m_Title)), graphicContext.DpiX(), graphicContext.DpiY())
		.Offset(allocatedArea.Left(), allocatedArea.Top());

	auto topMemorySizeRect =
		SMetricRectangleF(graphicContext.FontMetricsLarge().size(Qt::TextSingleLine,QString::fromStdString(this->m_MemorySizeText)), graphicContext.DpiX(), graphicContext.DpiY())
		.Offset(allocatedArea.Left() + topMemoryLabelRect.Width() + titleToSizeTextHSpaceMm, allocatedArea.Top());

	this->SetTitleArea(topMemoryLabelRect);
	this->SetMemorySizeTextArea(topMemorySizeRect);

	auto fontMetricsLargeHeightInPixels = graphicContext.FontMetricsLarge().height();
	currentYHolder += Graphical::GetMetricFromPixels(graphicContext.DpiY(), fontMetricsLargeHeightInPixels) + textToMemoryRegionVSpaceMm;
	double memoryRegionTop = currentYHolder;

	if (!this->m_ChildContent.empty())
	{
		currentYHolder += distanceFromFirstSectionToRegionTopInMmm;
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			auto childAllocatedArea = SMetricRectangleF(allocatedArea.Left(), currentYHolder, allocatedArea.Width(), childDesiredSize.CY());
			child->SetBodyPosition(childAllocatedArea, graphicContext);
			currentYHolder += childDesiredSize.CY() + sectionToSectionVSpaceMm;
		}
	}

	double memoryRegionBottom = currentYHolder;
	double memoryAreaHeight = memoryRegionBottom - memoryRegionTop;

	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), memoryRegionTop, allocatedArea.Width(), memoryAreaHeight));

	// Set size-marker and address marker
	CAddressedRegion::SetBodyPosition(this->BodyArea(), graphicContext);
}