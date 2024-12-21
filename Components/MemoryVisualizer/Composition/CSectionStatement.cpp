#include "CSectionStatement.h"
#include "CProgramHeaderButton.h"
#include "CFillExpressionButton.h"
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

constexpr double headerBoxHeight = 5;
constexpr double minimumContentAreaHeight = 4;

SMetricSizeF CSectionStatement::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	// Constants
	double minimumSectionStatementBoxHeight = headerBoxHeight; // 4mm header + 4mm empty boxy content

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
			minimumWidth += programHeader->CalculateBodySize(graphicContext).CX();
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
		calculatedHeight += marginSectionOutputFromTop;

		for (const auto& child : this->m_ChildOutputs)
		{
			auto childDesiredSize = child->CalculateBodySize(graphicContext);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + marginSectionOutputSpacing;
		}
	}
	else
	{
		// We add this here so the statement would have some space under it when empty.
		calculatedHeight += minimumContentAreaHeight;
	}

	return SMetricSizeF( std::max(maxContentWidth, minimumWidth), calculatedHeight);
}

void CSectionStatement::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
	auto currentYHolder = allocatedArea.Top();

	this->SetHeaderArea(SMetricRectangleF(allocatedArea.Left(), currentYHolder, allocatedArea.Width(), headerBoxHeight));
	
	auto calculatedTopLabelRect = SMetricRectangleF(graphicContext.FontMetricsSmall().size(Qt::TextSingleLine, QString::fromStdString(this->Title())), graphicContext.DpiX(), graphicContext.DpiY());
	auto topLabelRect = SMetricRectangleF(
			this->HeaderArea().Left() + marginTextFromHeaderLeft,
			this->HeaderArea().Top(),
			calculatedTopLabelRect.Width(),
			this->HeaderArea().Height());

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

	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), allocatedArea.Top(), allocatedArea.Width(), allocatedArea.Height()));

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

		programHeaderRightXPos -= calculatedProgramHeader.CX() - programHeaderSpacing;
	}

	// Set address start and top markers		
	CAddressedRegion::SetBodyPosition(this->BodyArea(), graphicContext);
}

void CSectionStatement::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	// Draw the addressed region
	const auto borderPen = QPen(QColor::fromRgba(Colors::SectionStatementDefaultBorderColor), 0, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(Colors::SectionStatementDefaultBackgroundColor), Qt::SolidPattern);
	this->PaintAddressedRegion(graphicContext, painter, borderPen, fillBrush);

	// Draw header	
	painter.fillRect(this->HeaderArea().ConvertToQRect(graphicContext), QBrush(QColor::fromRgba(Colors::SectionStatementHeaderBackgroundColor), Qt::SolidPattern));

	// Draw section name
	const auto titlePen = QPen(QColor::fromRgba(Colors::SectionStatementDefaultForeColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	painter.setPen(titlePen);
	painter.setFont(graphicContext.FontSmallBold());
	painter.drawText(this->TitleArea().ConvertToQRect(graphicContext), Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(this->Title()));

	// Draw program headers
	if (this->FillExpression().Defined())
	{
		this->FillExpression().Paint(graphicContext, painter);
	}

	for (const auto& progHeader : this->ProgramHeaders())
	{
		progHeader->Paint(graphicContext, painter);
	}

	// Draw all children
	for (const auto& childOutput : this->ChildOutputs())
	{
		childOutput->Paint(graphicContext, painter);
	}
}