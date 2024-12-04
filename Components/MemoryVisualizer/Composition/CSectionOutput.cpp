#include "CSectionOutput.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double marginTextToLeftInMm = 1;
constexpr double marginTextFromTopInMm = 1;


SMetricSizeF CSectionOutput::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{	
	const auto titleBoundingRect = fontMetricsSmall.boundingRect(QString::fromStdString(this->Content()));
	const auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingRect, dpiX, dpiY);
	const double calculatedWidth = titleBoundingRectMetric.Width() * 1.5;
	const double calculatedHeight = titleBoundingRectMetric.Height();
	return SMetricSizeF(
		calculatedWidth + ( 2 * marginTextToLeftInMm),
		calculatedHeight + ( 2 * marginTextFromTopInMm));
}

void CSectionOutput::SetBodyPosition(
	SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY, 
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	this->SetBodyArea(allocatedArea);
	auto titleBoundingRect = fontMetricsSmall.boundingRect(QString::fromStdString(this->Content()));
	auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingRect, dpiX, dpiY);
	auto alignedBoundingRectMetric = titleBoundingRectMetric.Offset(allocatedArea.Left() + marginTextToLeftInMm, allocatedArea.Top() + marginTextFromTopInMm);
	this->SetContentArea(alignedBoundingRectMetric);
}

void CSectionOutput::Paint(const QPainter& painter)
{

}