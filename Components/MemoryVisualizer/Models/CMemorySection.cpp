#include "CMemorySection.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

SMetricSizeF CMemorySection::CalculateDesiredSize(
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge )
{	
	// Constants
	double minimumMemoryBoxHeight = 10 + 4; // 14mm minimum memory box height. 4mm Text Label + 1mm Space + 1mm Border + 7mm Empty Space + 1mm border

	// Top memory label
	auto topMemoryLabel = Graphical::GetTextWidth("Memory Size: 16382 KB", fontMetricsLarge);	

	// Left size address text
	auto leftAddressTextSize = Graphical::GetTextWidth("0x0000000000000000", fontMetricsSmall);
	double leftAddressLine = 10; // 10mm = 1mm Space + 8mm Line + 1mm Space
	double leftSizeTotal = leftAddressTextSize + leftAddressLine;

	// Right side memory size lines
	double rightSizeLine1 = 40; // 40mm
	double rightSizeLine2 = 5; // 5mma
	double rightSizeText = Graphical::GetTextWidth("4096 Bytes", fontMetricsSmall);
	double rightSizeTotal = rightSizeLine1 + rightSizeLine2 + rightSizeText;
		
	double calculatedWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);
	double calculatedHeight = minimumMemoryBoxHeight;

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateDesiredSize(fontMetricsSmall, fontMetricsLarge);
			calculatedWidth = std::max(calculatedWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY();
		}
	}

	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CMemorySection::SetGeometry(SMetricRectangleF allocatedArea)
{
	
}
