#include "COverlaySectionOutput.h"


using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

SMetricSizeF COverlaySectionOutput::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	double minimumMemoryBoxHeight = 1 + 1 + 2; // 1mm top border, 1mm bottom border, 2mm text height	
	auto contentWidth = Graphical::GetTextWidthInPixels(this->m_Content, fontMetricsSmall);
	double calculatedWidth = contentWidth * 1.5;
	double calculatedHeight = minimumMemoryBoxHeight;
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void COverlaySectionOutput::SetBodyPosition(
	SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY, 
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{

}
