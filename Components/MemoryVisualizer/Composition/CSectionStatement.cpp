#include "CSectionStatement.h"
#include "CProgramHeaderButton.h"
#include "CFillExpressionButton.h"

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

SMetricSizeF CSectionStatement::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	// Constants
	double minimumSectionStatementBoxHeight = headerBoxHeight + minimumContentAreaHeight; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto topMemoryLabel = Graphical::GetMetricFromPixels(graphicContext.DpiX(), Graphical::GetTextWidthInPixels(this->Title(), graphicContext.FontMetricsSmall()));

	// Left size address text
	auto leftAddressTextSize = Graphical::GetMetricFromPixels(graphicContext.DpiX(), Graphical::GetTextWidthInPixels(this->AddressStartText(), graphicContext.FontMetricsSmall()));
	double leftAddressLine = 10; // 10mm = 1mm Space + 8mm Line + 1mm Space
	double leftSizeTotal = leftAddressTextSize + leftAddressLine;

	// Right side memory size lines
	double rightSizeLine1 = 5;
	double rightSizeLine2 = 5; // 5mm
	double rightSizeText = Graphical::GetMetricFromPixels(graphicContext.DpiX(), Graphical::GetTextWidthInPixels(this->SizeMarkerText(), graphicContext.FontMetricsSmall()));
	double rightSizeTotal = rightSizeLine1 + rightSizeLine2 + rightSizeText;

	double minimumWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);

	if (!this->ProgramHeaders().empty())
	{
		for (const auto& programHeader : this->ProgramHeaders())
		{
			minimumWidth += programHeader.CalculateBodySize(graphicContext).CX();
		}
	}

	if (this->FillExpression().Defined())
	{
		minimumWidth += this->FillExpression().CalculateBodySize(graphicContext).CX();
	}

	double calculatedHeight = minimumSectionStatementBoxHeight;
	double maxContentWidth = 0;

	if (!this->m_ChildOutputs.empty())
	{
		calculatedHeight -= minimumContentAreaHeight + marginSectionOutputFromTop;

		for (const auto& child : this->m_ChildOutputs)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + marginSectionOutputSpacing;
		}
	}

	return SMetricSizeF( std::max(maxContentWidth, minimumWidth), calculatedHeight);
}

void CSectionStatement::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
	auto currentYHolder = allocatedArea.Top();

	this->SetHeaderArea(SMetricRectangleF(allocatedArea.Left(), currentYHolder, allocatedArea.Width(), headerBoxHeight));
	
	auto topLabelRect =
		SMetricRectangleF(graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->Title())), graphicContext.DpiX(), graphicContext.DpiY())
		.Offset(allocatedArea.Left() + marginTextFromHeaderLeft, this->HeaderArea().Top() + marginTextFromHeaderTop);

	this->SetTitleArea(topLabelRect);

	currentYHolder += headerBoxHeight + marginSectionOutputFromTop;	
	auto sectionStatementTop = allocatedArea.Top();

	if (!this->m_ChildOutputs.empty())
	{
		for (const auto& child : this->m_ChildOutputs)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			child->SetBodyPosition(
				SMetricRectangleF(allocatedArea.Left() + marginSectionOutputLeft, currentYHolder, allocatedArea.Width() - (2*marginSectionOutputLeft) , childDesiredSize.CY()),
				graphicContext);

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
		auto calculatedProgramHeader = programHeader.CalculateBodySize(graphicContext);
		programHeader.SetBodyPosition(
			SMetricRectangleF(
				programHeaderRightXPos - calculatedProgramHeader.CX(),
				programHeaderTopYPos,
				calculatedProgramHeader.CX(),
				calculatedProgramHeader.CY()),
			graphicContext);

		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}

	if (this->FillExpression().Defined())
	{
		auto calculatedProgramHeader = this->FillExpression().CalculateBodySize(graphicContext);
		this->FillExpression().SetBodyPosition(
			SMetricRectangleF(
				programHeaderRightXPos - calculatedProgramHeader.CX(),
				programHeaderTopYPos,
				calculatedProgramHeader.CX(),
				calculatedProgramHeader.CY()),
			graphicContext);

		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}

	// Set address start and top markers		
	auto addressStartTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressStartText())));
	auto addressEndTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressEndText())));
	auto sizeMarkerTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->SizeMarkerText())));

	auto smallFontFullHeight = static_cast<double>(graphicContext.FontMetricsSmall().height());
	auto smallFontHalfHeight = static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2;

	this->SetAddressStartTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressStartTextWidth - addrssStartTextToLineHSpaceMm,
			sectionStatementTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight),
			addressStartTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontFullHeight)));

	this->SetAddressStartConnectingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight)));

	this->SetAddressEndTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressEndTextWidth - addrssStartTextToLineHSpaceMm,
			sectionStatementTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight),
			addressEndTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontFullHeight)));

	this->SetAddressEndConnctingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressEndTextWidth,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight)));

	// Set size marker
	this->SetSizeMarkerTextArea(
		SMetricRectangleF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			sectionStatementTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHalfHeight),
			sizeMarkerTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontFullHeight)));

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

void CSectionStatement::Paint(
	const CGraphicContext& graphicContext,
	const QPainter& painter)
{

}