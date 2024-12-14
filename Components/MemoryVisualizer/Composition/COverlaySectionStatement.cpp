#include "COverlaySectionStatement.h"

#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double marginTextFromHeaderTop = 1;
constexpr double marginTextFromHeaderBottom = 1;
constexpr double marginTextFromHeaderLeft = 1;

constexpr double marginSectionOutputFromTop = 1;
constexpr double marginSectionOutputSpacing = 1;
constexpr double marginSectionOutputLeft = 1;

constexpr double addressStartConnectingLineLengthMm = 10;
constexpr double addrssStartTextToLineHSpaceMm = 5;
constexpr double sizeMarkerFirstLineLengthMm = 5;
constexpr double sizeMarkerSecondLineLengthMm = 5;

constexpr double programHeaderMarginTop = 0.5;
constexpr double programHeaderMarginBottom = 0.5;
constexpr double programHeaderMarginRight = 1;
constexpr double programHeaderSpacing = 1;

constexpr double headerBoxHeight = 4;
constexpr double minimumContentAreaHeight = 4;

SMetricSizeF COverlaySectionStatement::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	// Constants
	double minimumSectionStatementBoxHeight = headerBoxHeight + minimumContentAreaHeight; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto topMemoryLabel = Graphical::GetMetricFromPixels(graphicContext.DpiX(), Graphical::GetTextWidthInPixels(this->Title(), graphicContext.FontMetricsSmall()));
	double minimumWidth = topMemoryLabel * 1.5;

	if (!this->ProgramHeaders().empty())
	{
		for (const auto& programHeader : this->ProgramHeaders())
		{
			minimumWidth += programHeader->CalculateBodySize(graphicContext).CX();
		}
	}

	if (this->FillExpression().Defined())
	{
		minimumWidth += this->FillExpression().CalculateBodySize(graphicContext).CX();
	}

	double calculatedHeight = minimumSectionStatementBoxHeight;
	double maxContentWidth = 0;

	if (!this->m_ChildContent.empty())
	{
		calculatedHeight -= minimumContentAreaHeight + marginSectionOutputFromTop;

		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + marginSectionOutputSpacing;
		}
	}

	return SMetricSizeF(std::max(maxContentWidth, minimumWidth), calculatedHeight);
}

void COverlaySectionStatement::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
	auto currentYHolder = allocatedArea.Top();

	this->SetHeaderArea(SMetricRectangleF(allocatedArea.Left(), currentYHolder, allocatedArea.Width(), headerBoxHeight));

	auto topLabelRect =
		SMetricRectangleF(graphicContext.FontMetricsLarge().boundingRect(QString::fromStdString(this->Title())), graphicContext.DpiX(), graphicContext.DpiY())
		.Offset(allocatedArea.Left() + marginTextFromHeaderLeft, this->HeaderArea().Top() + marginTextFromHeaderTop);

	this->SetTitleArea(topLabelRect);

	currentYHolder += headerBoxHeight + marginSectionOutputFromTop;
	auto sectionStatementTop = allocatedArea.Top();

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			auto childAllocatedArea = SMetricRectangleF(
				allocatedArea.Left() + marginSectionOutputLeft,
				currentYHolder, 
				allocatedArea.Width() - (2 * marginSectionOutputLeft),
				childDesiredSize.CY());

			child->SetBodyPosition(childAllocatedArea, graphicContext);
			currentYHolder += childDesiredSize.CY() + marginSectionOutputSpacing;
		}
	}

	double sectionStatementBottom = currentYHolder;
	double sectionStatementHeight = sectionStatementBottom - allocatedArea.Top();

	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), allocatedArea.Top(), calculatedDesiredSize.CX(), sectionStatementHeight));

	// Set fill-expression and program-headers
	auto programHeaderTopYPos = allocatedArea.Top() + programHeaderMarginTop;
	auto programHeaderRightXPos = allocatedArea.Right() - programHeaderMarginRight;

	for (auto programHeader : this->ProgramHeaders())
	{
		auto calculatedProgramHeader = programHeader->CalculateBodySize(graphicContext);
		auto programHeaderAllocatedArea = SMetricRectangleF(
			programHeaderRightXPos - calculatedProgramHeader.CX(),
			programHeaderTopYPos,
			calculatedProgramHeader.CX(),
			calculatedProgramHeader.CY());

		programHeader->SetBodyPosition(programHeaderAllocatedArea, graphicContext);
		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}

	if (this->FillExpression().Defined())
	{
		auto calculatedProgramHeader = this->FillExpression().CalculateBodySize(graphicContext);
		auto fillExpressionAllocatedArea = SMetricRectangleF(
			programHeaderRightXPos - calculatedProgramHeader.CX(),
			programHeaderTopYPos,
			calculatedProgramHeader.CX(),
			calculatedProgramHeader.CY());

		this->FillExpression().SetBodyPosition(fillExpressionAllocatedArea, graphicContext);
		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}
}

void COverlaySectionStatement::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	// Draw the addressed region
	const auto borderPen = QPen(QColor::fromRgb(Colors::OverlayStatementDefaultBorderColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(Colors::OverlayStatementDefaultBackgroundColor), Qt::SolidPattern);
	this->PaintAddressedRegion(graphicContext, painter, borderPen, fillBrush);

	// Draw overlay section name
	painter.setFont(graphicContext.FontSmallBold());
	painter.drawText(
		this->TitleArea().ConvertToQRect(graphicContext),
		Qt::AlignHCenter | Qt::AlignVCenter,
		QString::fromStdString(this->Title()));

	// Draw program headers
	if (this->FillExpression().Defined())
	{
		this->FillExpression().Paint(graphicContext, painter);
	}

	for (auto progHeader : this->ProgramHeaders())
	{
		progHeader->Paint(graphicContext, painter);
	}

	// Draw all children
	for (const auto& childContent : this->ChildContent())
	{
		childContent->Paint(graphicContext, painter);
	}
}