#include "CSectionOutput.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

SMetricSizeF CSectionOutput::CalculateDesiredSize(
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{	
	double minimumMemoryBoxHeight = 1 + 1 + 2; // 1mm top border, 1mm bottom border, 2mm text height	
	auto contentWidth = Graphical::GetTextWidth(this->m_Content, fontMetricsSmall);
	double calculatedWidth = contentWidth * 1.5;
	double calculatedHeight = minimumMemoryBoxHeight;	
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CSectionOutput::SetGeometry(SMetricRectangleF allocatedArea)
{

}
