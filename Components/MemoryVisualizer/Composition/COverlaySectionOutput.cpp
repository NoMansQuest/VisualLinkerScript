#include "COverlaySectionOutput.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double marginTextToLeftInMm = 1;
constexpr double marginTextFromTopInMm = 1;

SMetricSizeF COverlaySectionOutput::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	const auto titleBoundingRect = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->Content()));
	const auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingRect, graphicContext.DpiX(), graphicContext.DpiY());
	const double calculatedWidth = titleBoundingRectMetric.Width() * 1.5;
	const double calculatedHeight = titleBoundingRectMetric.Height();
	return SMetricSizeF(
		calculatedWidth + (2 * marginTextToLeftInMm),
		calculatedHeight + (2 * marginTextFromTopInMm));
}

void COverlaySectionOutput::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	this->SetBodyArea(allocatedArea);
	auto titleBoundingRect = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->Content()));
	auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingRect, graphicContext.DpiX(), graphicContext.DpiY());
	auto alignedBoundingRectMetric = titleBoundingRectMetric.Offset(allocatedArea.Left() + marginTextToLeftInMm, allocatedArea.Top() + marginTextFromTopInMm);
	this->SetContentArea(alignedBoundingRectMetric);
}

void COverlaySectionOutput::Paint(
	const CGraphicContext& graphicContext,
	const QPainter& painter)
{

}