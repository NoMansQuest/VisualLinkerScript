#include "COverlayStatement.h"
#include "CProgramHeaderButton.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

SMetricSizeF COverlayStatement::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	// Constants
	double minimumMemoryBoxHeight = 4 + 4; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto topMemoryLabel = Graphical::GetTextWidthInPixels(this->Title(), fontMetricsSmall);

	// Left size address text
	auto leftAddressTextSize = Graphical::GetTextWidthInPixels("0x0000000000000000", fontMetricsSmall);
	double leftAddressLine = 10; // 10mm = 1mm Space + 8mm Line + 1mm Space
	double leftSizeTotal = leftAddressTextSize + leftAddressLine;

	// Right side memory size lines
	double rightSizeLine1 = 5;
	double rightSizeLine2 = 5; // 5mm
	double rightSizeText = Graphical::GetTextWidthInPixels("4096 Bytes", fontMetricsSmall);
	double rightSizeTotal = rightSizeLine1 + rightSizeLine2 + rightSizeText;

	double minimumWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);

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

	double maxContentHeight = 0;
	double cumulativeSectionWidth = 0;	

	if (!this->m_OverlaySections.empty())
	{		
		for (const auto& overlaySection : this->m_OverlaySections)
		{
			auto childDesiredSize = overlaySection->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
			cumulativeSectionWidth += childDesiredSize.CX() + 1; // 1mm spacing			
			maxContentHeight = childDesiredSize.CY() > maxContentHeight ? childDesiredSize.CY() : maxContentHeight;
		}
	}

	return SMetricSizeF(
		cumulativeSectionWidth > minimumWidth ? cumulativeSectionWidth : minimumWidth,
		minimumMemoryBoxHeight + maxContentHeight);
}

void COverlayStatement::SetBodyPosition(
	SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{

}