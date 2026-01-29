#include "Components/MemoryVisualizer/Composition/CSectionOutput.h"

#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double marginTextToLeftInMm = 1;
constexpr double marginTextFromTopInMm = 1;

SMetricSizeF CSectionOutput::CalculateBodySize(const CGraphicContext& graphicContext) const
{	
	const auto titleBoundingRect = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->Content()));
	const auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingRect, graphicContext.DpiX(), graphicContext.DpiY());
	const double calculatedWidth = titleBoundingRectMetric.Width() * 1.5;
	const double calculatedHeight = titleBoundingRectMetric.Height();
	return SMetricSizeF(
		calculatedWidth + ( 2 * marginTextToLeftInMm),
		calculatedHeight + ( 2 * marginTextFromTopInMm));
}

void CSectionOutput::SetBodyPosition(
	const SMetricRectangleF& allocatedArea,
	const CGraphicContext& graphicContext)
{
	this->SetBodyArea(allocatedArea);
	auto titleBoundingRect = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->Content()));
	auto titleBoundingRectMetric = SMetricRectangleF(titleBoundingRect, graphicContext.DpiX(), graphicContext.DpiY());

	auto alignedBoundingRectMetric = SMetricRectangleF(
		allocatedArea.Left() + marginTextToLeftInMm, 
		allocatedArea.Top() + marginTextFromTopInMm,
		titleBoundingRectMetric.Width(),
		titleBoundingRectMetric.Height());

	this->SetContentArea(alignedBoundingRectMetric);
}

void CSectionOutput::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	// Draw surrounding rectangle
	painter.setPen(QPen(QColor::fromRgba(Colors::SectionOutputClickBorderColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
	painter.fillRect(this->BodyArea().ConvertToQRect(graphicContext), QBrush(QColor::fromRgba(Colors::SectionOutputDefaultBackgroundColor), Qt::SolidPattern));
	painter.drawRect(this->BodyArea().ConvertToQRect(graphicContext));

	// Draw section name
	painter.setPen(QColor::fromRgba(Colors::SectionOutputDefaultForeColor));
	painter.setFont(graphicContext.FontSmall());
	painter.drawText(
		this->ContentArea().ConvertToQRect(graphicContext),
		Qt::AlignHCenter | Qt::AlignVCenter,
		QString::fromStdString(this->Content()));
}
