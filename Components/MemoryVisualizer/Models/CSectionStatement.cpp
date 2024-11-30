#include "CSectionStatement.h"
#include "CProgramHeaderButton.h"
#include "CFillExpressionButton.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

SMetricSizeF CSectionStatement::CalculateDesiredSize(
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	// Constants
	double minimumMemoryBoxHeight = 4 + 4; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto topMemoryLabel = Graphical::GetTextWidth(this->Title(), fontMetricsSmall);

	// Left size address text
	auto leftAddressTextSize = Graphical::GetTextWidth("0x0000000000000000", fontMetricsSmall);
	double leftAddressLine = 10; // 10mm = 1mm Space + 8mm Line + 1mm Space
	double leftSizeTotal = leftAddressTextSize + leftAddressLine;

	// Right side memory size lines
	double rightSizeLine1 = 5;
	double rightSizeLine2 = 5; // 5mm
	double rightSizeText = Graphical::GetTextWidth("4096 Bytes", fontMetricsSmall);
	double rightSizeTotal = rightSizeLine1 + rightSizeLine2 + rightSizeText;

	double minimumWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);

	if (!this->ProgramHeaders().empty())
	{
		for (const auto& programHeader : this->ProgramHeaders())
		{
			minimumWidth += programHeader.CalculateDesiredSize(fontMetricsSmall, fontMetricsLarge).CX();
		}
	}

	if (this->FillExpression().Defined())
	{
		minimumWidth += this->FillExpression().CalculateDesiredSize(fontMetricsSmall, fontMetricsLarge).CX();
	}

	double calculatedHeight = minimumMemoryBoxHeight;
	double maxContentWidth = 0;

	if (!this->m_ChildOutputs.empty())
	{
		for (const auto child : this->m_ChildOutputs)
		{
			auto childDesiredSize = child->CalculateDesiredSize(fontMetricsSmall, fontMetricsLarge);
			maxContentWidth = std::max(maxContentWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY();
		}
	}

	return SMetricSizeF( std::max(maxContentWidth, minimumWidth), calculatedHeight);
}

void CSectionStatement::SetGeometry(SMetricRectangleF allocatedArea)
{

}
