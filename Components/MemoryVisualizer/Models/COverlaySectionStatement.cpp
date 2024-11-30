#include "COverlaySectionStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

SMetricSizeF COverlaySectionStatement::CalculateDesiredSize(
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	// Constants
	double minimumMemoryBoxHeight = 4 + 4; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto sectionStatementLabel = Graphical::GetTextWidth(this->Name(), fontMetricsSmall);
	double calculatedWidth = (sectionStatementLabel * 1.5);
	double calculatedHeight = minimumMemoryBoxHeight;

	if (!this->m_ChildContent.empty())
	{
		for (const auto child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateDesiredSize(fontMetricsSmall, fontMetricsLarge);
			calculatedWidth = std::max(calculatedWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY();
		}
	}

	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void COverlaySectionStatement::SetGeometry(SMetricRectangleF allocatedArea)
{

}
