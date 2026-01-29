#include "Components/MemoryVisualizer/Composition/COverlaySectionOutput.h"
#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double marginTextToLeftInMm = 1;
constexpr double marginTextFromTopInMm = 1;
constexpr double sizeMarkerFirstLineLengthMm = 0;

SMetricSizeF COverlaySectionOutput::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	const auto titleBoundingSize = graphicContext.FontMetricsSmall().size(Qt::TextSingleLine, QString::fromStdString(this->Content()));
	const auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingSize, graphicContext.DpiX(), graphicContext.DpiY());
	const double calculatedWidth = titleBoundingRectMetric.Width() * 1.5;
	const double calculatedHeight = titleBoundingRectMetric.Height();
	return SMetricSizeF(
		calculatedWidth + (2 * marginTextToLeftInMm),
		calculatedHeight + (2 * marginTextFromTopInMm));
}

void COverlaySectionOutput::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	this->SetBodyArea(allocatedArea);
	auto titleBoundingSize = graphicContext.FontMetricsSmall().size(Qt::TextSingleLine, QString::fromStdString(this->Content()));
	auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingSize, graphicContext.DpiX(), graphicContext.DpiY());
	auto alignedBoundingRectMetric = titleBoundingRectMetric.Offset(allocatedArea.Left() + marginTextToLeftInMm, allocatedArea.Top() + marginTextFromTopInMm);
	this->SetContentArea(alignedBoundingRectMetric);
}

void COverlaySectionOutput::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	// NOTE: For OverlaySectionOutput, we use 'SectionOutput' style (they're the same in essence).
	// Draw the addressed region
	const auto borderPen = QPen(QColor::fromRgba(Colors::SectionOutputClickBorderColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(Colors::SectionOutputDefaultBackgroundColor), Qt::SolidPattern);
	this->PaintAddressedRegion(graphicContext, painter, borderPen, fillBrush);

	// Draw section name
	painter.setPen(QColor::fromRgba(Colors::SectionOutputClickForeColor));
	painter.setFont(graphicContext.FontSmall());
	painter.drawText(
		this->ContentArea().ConvertToQRect(graphicContext),
		Qt::AlignHCenter | Qt::AlignVCenter,
		QString::fromStdString(this->Content()));
}
