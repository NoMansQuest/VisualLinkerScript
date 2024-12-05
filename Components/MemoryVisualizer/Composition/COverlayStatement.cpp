#include "COverlayStatement.h"
#include "CProgramHeaderButton.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double marginTextFromHeaderTop = 1;
constexpr double marginTextFromHeaderBottom = 1;
constexpr double marginTextFromHeaderLeft = 1;

constexpr double marginSectionOutputFromTop = 1;
constexpr double marginSectionOutputSpacing = 1;
constexpr double marginSectionOutputLeft = 1;
constexpr double marginSectionOutputFromBottom = 1; 

constexpr double addressStartConnectingLineLengthMm = 10;
constexpr double addressStartTextToLineHSpaceMm = 5;
constexpr double sizeMarkerFirstLineLengthMm = 5;
constexpr double sizeMarkerSecondLineLengthMm = 5;

constexpr double programHeaderMarginTop = 0.5;
constexpr double programHeaderMarginBottom = 0.5;
constexpr double programHeaderMarginRight = 1;
constexpr double programHeaderSpacing = 1;

constexpr double headerBoxHeight = 4;
constexpr double minimumContentAreaHeight = 4;


SMetricSizeF COverlayStatement::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	// Constants
	double minimumMemoryBoxHeight = headerBoxHeight + minimumContentAreaHeight;

	// Top memory label
	auto topMemoryLabel = Graphical::GetTextWidthInPixels(this->Title(), graphicContext.FontMetricsSmall());

	// Left size address text
	auto addressTextSize = Graphical::GetMetricFromPixels(graphicContext.DpiX(), Graphical::GetTextWidthInPixels(this->AddressStartText(), graphicContext.FontMetricsSmall()));
	double leftSizeTotal = addressTextSize + addressStartConnectingLineLengthMm;

	// Right side memory size lines	
	double sizeMarkerTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), Graphical::GetTextWidthInPixels(this->SizeMarkerText(), graphicContext.FontMetricsSmall()));
	double rightSizeTotal = sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm + sizeMarkerTextWidth;

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

	double maxContentHeight = 0;	
	double finalContentWidthWithMargin = 0;

	if (!this->m_OverlaySections.empty())
	{
		double maxContentWidth = 0;

		for (const auto& overlaySection : this->m_OverlaySections)
		{
			auto childDesiredSize = overlaySection->CalculateBodySize(graphicContext);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX()); // 1mm spacing			
			maxContentHeight = childDesiredSize.CY() > maxContentHeight ? childDesiredSize.CY() : maxContentHeight;
		}

		finalContentWidthWithMargin = 
			(maxContentWidth * static_cast<double>(this->m_OverlaySections.size())) + 
			((static_cast<double>(this->m_OverlaySections.size()) - 1.0) * marginSectionOutputSpacing);
	}	

	return SMetricSizeF(
		finalContentWidthWithMargin > minimumWidth ? finalContentWidthWithMargin : minimumWidth,
		minimumMemoryBoxHeight + maxContentHeight);
}

void COverlayStatement::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
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

	if (!this->m_OverlaySections.empty())
	{
		double maxContentWidth = 0;
		double maxDetectedHeight = 0;
		double currentXHolder = allocatedArea.Left() + marginSectionOutputLeft;

		for (const auto& overlaySection : this->m_OverlaySections)
		{
			auto childDesiredSize = overlaySection->CalculateBodySize(graphicContext);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX()); // 1mm spacing						
		}

		for (const auto& overlaySection : this->m_OverlaySections)
		{
			auto childDesiredSize = overlaySection->CalculateBodySize(graphicContext);
			maxDetectedHeight = std::max(maxDetectedHeight, childDesiredSize.CY());
			overlaySection->SetBodyPosition(
				SMetricRectangleF(currentXHolder, currentYHolder, maxContentWidth, childDesiredSize.CY()),
				graphicContext);

			currentXHolder += maxContentWidth + marginSectionOutputSpacing;
		}

		currentYHolder += maxDetectedHeight + marginSectionOutputFromBottom;
	}

	double overlayStatementBottom = currentYHolder;
	double overlayStatementHeight = overlayStatementBottom - allocatedArea.Top();

	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), allocatedArea.Top(), calculatedDesiredSize.CX(), overlayStatementHeight));

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
	}

	// Set address start and top markers		
	auto addressStartTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressStartText())));
	auto addressEndTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressEndText())));
	auto sizeMarkerTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->SizeMarkerText())));

	this->SetAddressStartTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressStartTextWidth - addressStartTextToLineHSpaceMm,
			sectionStatementTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			addressStartTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), graphicContext.FontMetricsSmall().height())));

	this->SetAddressStartConnectingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2)));

	this->SetAddressEndTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressEndTextWidth - addressStartTextToLineHSpaceMm,
			sectionStatementTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			addressEndTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), graphicContext.FontMetricsSmall().height())));

	this->SetAddressEndConnctingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressEndTextWidth,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2)));

	// Set size marker
	this->SetSizeMarkerTextArea(
		SMetricRectangleF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			sectionStatementTop - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			sizeMarkerTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), graphicContext.FontMetricsSmall().height())));

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

void COverlayStatement::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{

}