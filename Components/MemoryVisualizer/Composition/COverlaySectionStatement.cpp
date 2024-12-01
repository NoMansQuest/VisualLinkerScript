#include "COverlaySectionStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

SMetricSizeF COverlaySectionStatement::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	// Constants
	double minimumMemoryBoxHeight = 4 + 4; // 4mm header + 4mm empty boxy content

	// Top memory label
	auto sectionStatementLabel = Graphical::GetTextWidthInPixels(this->Name(), fontMetricsSmall);
	double calculatedContentWidth = (sectionStatementLabel * 1.5);
	double calculatedHeight = minimumMemoryBoxHeight;

	double minimumWidth = sectionStatementLabel;

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

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
			calculatedContentWidth = std::max(calculatedContentWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY();
		}
	}

	return SMetricSizeF(std::max(calculatedContentWidth, minimumWidth), calculatedHeight);
}

void COverlaySectionStatement::SetBodyPosition(
	SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY, 
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{

}
