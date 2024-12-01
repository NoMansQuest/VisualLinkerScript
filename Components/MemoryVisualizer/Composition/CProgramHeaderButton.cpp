#include "CProgramHeaderButton.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

SMetricSizeF CProgramHeaderButton::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{	
	auto contentWidth = Graphical::GetTextWidthInPixels(this->m_ProgramHeader, fontMetricsSmall);
	double calculatedWidth = contentWidth + 1 + 1; // 1mm left margin, 1mm right margin
	double calculatedHeight = 1 + 2 + 1;  // 1mm top margin, 2mm text, 1mm right margin
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CProgramHeaderButton::SetBodyPosition(
	SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY, 
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{

}