#include "COverlaySectionStatement.h"

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

SMetricSizeF COverlaySectionStatement::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	// Constants
	double minimumSectionStatementBoxHeight = headerBoxHeight + minimumContentAreaHeight; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto topMemoryLabel = Graphical::GetMetricFromPixels(dpiX, Graphical::GetTextWidthInPixels(this->Title(), fontMetricsSmall));
	double minimumWidth = topMemoryLabel * 1.5;

	if (!this->ProgramHeaders().empty())
	{
		for (const auto& programHeader : this->ProgramHeaders())
		{
			minimumWidth += programHeader.CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge).CX();
		}
	}

	if (this->FillExpression().Defined())
	{
		minimumWidth += this->FillExpression().CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge).CX();
	}

	double calculatedHeight = minimumSectionStatementBoxHeight;
	double maxContentWidth = 0;

	if (!this->m_ChildContent.empty())
	{
		calculatedHeight -= minimumContentAreaHeight + marginSectionOutputFromTop;

		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + marginSectionOutputSpacing;
		}
	}

	return SMetricSizeF(std::max(maxContentWidth, minimumWidth), calculatedHeight);
}

void COverlaySectionStatement::SetBodyPosition(
	SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY, 
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	auto calculatedDesiredSize = this->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
	auto currentYHolder = allocatedArea.Top();

	this->SetHeaderArea(SMetricRectangleF(allocatedArea.Left(), currentYHolder, allocatedArea.Width(), headerBoxHeight));

	auto topLabelRect =
		SMetricRectangleF(fontMetricsLarge.boundingRect(QString::fromStdString(this->Title())), dpiX, dpiY)
		.Offset(allocatedArea.Left() + marginTextFromHeaderLeft, this->HeaderArea().Top() + marginTextFromHeaderTop);

	this->SetTitleArea(topLabelRect);

	currentYHolder += headerBoxHeight + marginSectionOutputFromTop;
	auto sectionStatementTop = allocatedArea.Top();

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
			child->SetBodyPosition(
				SMetricRectangleF(allocatedArea.Left() + marginSectionOutputLeft, currentYHolder, allocatedArea.Width() - (2 * marginSectionOutputLeft), childDesiredSize.CY()),
				dpiX,
				dpiY,
				fontMetricsSmall,
				fontMetricsLarge);

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
		auto calculatedProgramHeader = programHeader.CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		programHeader.SetBodyPosition(
			SMetricRectangleF(
				programHeaderRightXPos - calculatedProgramHeader.CX(),
				programHeaderTopYPos,
				calculatedProgramHeader.CX(),
				calculatedProgramHeader.CY()),
			dpiX,
			dpiY,
			fontMetricsSmall,
			fontMetricsLarge);

		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}

	if (this->FillExpression().Defined())
	{
		auto calculatedProgramHeader = this->FillExpression().CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		this->FillExpression().SetBodyPosition(
			SMetricRectangleF(
				programHeaderRightXPos - calculatedProgramHeader.CX(),
				programHeaderTopYPos,
				calculatedProgramHeader.CX(),
				calculatedProgramHeader.CY()),
			dpiX,
			dpiY,
			fontMetricsSmall,
			fontMetricsLarge);

		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}
}

void COverlaySectionStatement::Paint(const QPainter& painter)
{

}