#include "CFillExpressionButton.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

SMetricSizeF CFillExpressionButton::CalculateDesiredSize(
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{
	auto contentWidth = Graphical::GetTextWidth(this->m_FillExpression, fontMetricsSmall);
	double calculatedWidth = contentWidth + 1 + 1; // 1mm left margin, 1mm right margin
	double calculatedHeight = 1 + 2 + 1;  // 1mm top margin, 2mm text, 1mm right margin
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CFillExpressionButton::SetGeometry(SMetricRectangleF allocatedArea)
{

}
