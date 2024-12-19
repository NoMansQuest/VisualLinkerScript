#include "COverlayStatement.h"
#include "CProgramHeaderButton.h"
#include "Components/MemoryVisualizer/ColorResources.h"

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

constexpr double headerBoxHeight = 5;
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

	double minimumVanillaWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);

	if (!this->ProgramHeaders().empty())
	{
		for (const auto& programHeader : this->ProgramHeaders())
		{
			minimumVanillaWidth += programHeader->CalculateBodySize(graphicContext).CX();
		}
	}

	if (this->FillExpression().Defined())
	{
		minimumVanillaWidth += this->FillExpression().CalculateBodySize(graphicContext).CX();
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
		std::max(finalContentWidthWithMargin , minimumVanillaWidth),
		minimumMemoryBoxHeight + maxContentHeight);
}

void COverlayStatement::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
	auto currentYHolder = allocatedArea.Top();

	this->SetHeaderArea(SMetricRectangleF(allocatedArea.Left(), currentYHolder, allocatedArea.Width(), headerBoxHeight));

	auto topLabelBoundsCalculated = SMetricRectangleF(graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->Title())), graphicContext.DpiX(), graphicContext.DpiY());
	auto topLabelRect = SMetricRectangleF(
		this->HeaderArea().Left() + marginTextFromHeaderLeft,
		this->HeaderArea().Top(),
		topLabelBoundsCalculated.Width(),
		this->HeaderArea().Height());

	this->SetTitleArea(topLabelRect);

	currentYHolder += headerBoxHeight + marginSectionOutputFromTop;
	auto sectionStatementTop = allocatedArea.Top();

	if (!this->m_OverlaySections.empty())
	{
		double childStatementWidth = allocatedArea.Width() / this->m_OverlaySections.size();
		double maxDetectedHeight = 0;
		double currentXHolder = allocatedArea.Left() + marginSectionOutputLeft;

		for (const auto& overlaySection : this->m_OverlaySections)
		{
			auto childDesiredSize = overlaySection->CalculateBodySize(graphicContext);
			maxDetectedHeight = std::max(maxDetectedHeight, childDesiredSize.CY());
			overlaySection->SetBodyPosition(
				SMetricRectangleF(currentXHolder, currentYHolder, childStatementWidth - marginSectionOutputSpacing, childDesiredSize.CY()),
				graphicContext);

			currentXHolder += childStatementWidth;
		}

		currentYHolder += maxDetectedHeight + marginSectionOutputFromBottom;
	}

	double overlayStatementBottom = currentYHolder;
	double overlayStatementHeight = overlayStatementBottom - allocatedArea.Top();

	// Set fill-expression and program-headers
	auto programHeaderTopYPos = allocatedArea.Top() + programHeaderMarginTop;
	auto programHeaderRightXPos = allocatedArea.Right() - programHeaderMarginRight;

	for (const auto& programHeader : this->ProgramHeaders())
	{
		auto calculatedProgramHeader = programHeader->CalculateBodySize(graphicContext);
		programHeader->SetBodyPosition(
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

	// Set the main body area.
	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), allocatedArea.Top(), allocatedArea.Width(), overlayStatementHeight));

	// Set address start and top markers		
	CAddressedRegion::SetBodyPosition(this->BodyArea(), graphicContext);
}

void COverlayStatement::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	// Draw the addressed region
	const auto borderPen = QPen(QColor::fromRgba(Colors::OverlayStatementClickBorderColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(Colors::SectionStatementDefaultBackgroundColor), Qt::SolidPattern);
	this->PaintAddressedRegion(graphicContext, painter, borderPen, fillBrush);

	// Draw header	
	painter.fillRect(this->HeaderArea().ConvertToQRect(graphicContext), QBrush(QColor::fromRgba(Colors::SectionStatementHeaderBackgroundColor), Qt::SolidPattern));

	// Draw section name
	painter.setPen(QColor::fromRgba(Colors::OverlayStatementDefaultForeColor));
	painter.setFont(graphicContext.FontSmallBold());
	painter.drawText(this->TitleArea().ConvertToQRect(graphicContext), Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(this->Title()));

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
	for (const auto& childOutput : this->OverlaySections())
	{
		childOutput->Paint(graphicContext, painter);
	}
}