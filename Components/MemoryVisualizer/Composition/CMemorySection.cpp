#include "CMemorySection.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double textToMemoryRegionVSpaceMm = 2;
constexpr double titleToSizeTextHSpaceMm = 2;
constexpr double memoryRegionBorderMm = 0.5;
constexpr double sectionToSectionVSpaceMm = 4;
constexpr double addressStartConnectingLineLengthMm = 10;
constexpr double addrssStartTextToLineHSpaceMm = 5;
constexpr double sizeMarkerFirstLineLengthMm = 40;
constexpr double sizeMarkerSecondLineLengthMm = 5;

SMetricSizeF CMemorySection::CalculateBodySize(const CGraphicContext& graphicContext) const
{	
	// Constants
	double minimumMemoryBoxHeight = 10 + 2; // 14mm minimum memory box height. 4mm Text Label + 2mm Space + 0.5mm Border + 7mm Empty Space + 0.5mm border

	// Top memory label
	auto topMemoryLabel = Graphical::GetTextWidthInPixels("Memory Size: 16382 KB", graphicContext.FontMetricsLarge());

	// Left size address text
	auto leftAddressTextSize = Graphical::GetTextWidthInPixels("0x0000000000000000", graphicContext.FontMetricsSmall());
	double leftAddressLine = addressStartConnectingLineLengthMm; // 10mm = 1mm Space + 8mm Line + 1mm Space
	double leftSizeTotal = leftAddressTextSize + leftAddressLine;

	// Right side memory size lines
	double rightSizeLine1 = sizeMarkerFirstLineLengthMm; 
	double rightSizeLine2 = sizeMarkerSecondLineLengthMm; 
	double rightSizeText = Graphical::GetTextWidthInPixels("4096 Bytes", graphicContext.FontMetricsSmall());
	double rightSizeTotal = rightSizeLine1 + rightSizeLine2 + rightSizeText;
		
	double calculatedWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);
	double calculatedHeight = minimumMemoryBoxHeight;

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			calculatedWidth = std::max(calculatedWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + sectionToSectionVSpaceMm;
		}
	}

	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CMemorySection::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) 
{
	auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
	auto currentYHolder = allocatedArea.Top();

	auto topMemoryLabelRect = 
		SMetricRectangleF(graphicContext.FontMetricsLarge().boundingRect(QString::fromStdString(this->m_Title)), graphicContext.DpiX(), graphicContext.DpiY())
		.Offset(allocatedArea.Left(), allocatedArea.Top());

	auto topMemorySizeRect = 
		SMetricRectangleF(graphicContext.FontMetricsLarge().boundingRect(QString::fromStdString(this->m_MemorySizeText)), graphicContext.DpiX(), graphicContext.DpiY())
		.Offset(allocatedArea.Left() + topMemoryLabelRect.Width() + titleToSizeTextHSpaceMm, allocatedArea.Top());

	this->SetTitleArea(topMemoryLabelRect);
	this->SetMemorySizeTextArea(topMemorySizeRect);

	currentYHolder += graphicContext.FontMetricsLarge().height() + textToMemoryRegionVSpaceMm;
	double memoryRegionTop = currentYHolder;

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			auto childAllocatedArea = SMetricRectangleF(allocatedArea.Left(), currentYHolder, childDesiredSize.CX(), childDesiredSize.CY());
			child->SetBodyPosition(childAllocatedArea, graphicContext);
			currentYHolder += childDesiredSize.CY() + sectionToSectionVSpaceMm;
		}
	}

	double memoryRegionBottom = currentYHolder;
	double memoryAreaHeight = memoryRegionBottom - memoryRegionTop;

	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), memoryRegionTop, calculatedDesiredSize.CX(), memoryAreaHeight));

	// Set address start and top markers		
	auto addressStartTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressStartText())));
	auto addressEndTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressEndText())));
	auto sizeMarkerTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->SizeMarkerText())));

	auto smallFontHeightHalf = static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2;
	auto smallFontHeight = static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2;

	this->SetAddressStartTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressStartTextWidth - addrssStartTextToLineHSpaceMm,
			memoryRegionTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			addressStartTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeight)));

	this->SetAddressStartConnectingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf)));

	this->SetAddressEndTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressEndTextWidth - addrssStartTextToLineHSpaceMm,
			memoryRegionTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			addressEndTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeight)));

	this->SetAddressEndConnctingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressEndTextWidth,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf)));
	
	// Set size marker
	this->SetSizeMarkerTextArea(
		SMetricRectangleF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			memoryRegionTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			sizeMarkerTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeight)));

	this->SetSizeMarkerUpperConnector(
		SLineF(
			this->BodyArea().Right(),
			this->BodyArea().Top(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top()));

	this->SetSizeMarkerUpperConnector(
		SLineF(
			this->BodyArea().Right(),
			this->BodyArea().Bottom(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom()));

	this->SetSizeMarkerVerticalLine(
		SLineF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom()));

	this->SetSizeMarkerCenterConnector(
		SLineF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Center().Y(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			this->BodyArea().Center().Y()));
}

void CMemorySection::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{

}