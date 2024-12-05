#include "CFillExpressionButton.h"
#include "CGraphicContext.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double textMarginLeft = 1;
constexpr double textMarginRight = 1;
constexpr double textMarginBottom = 1;
constexpr double textMarginTop = 1;

SMetricSizeF CFillExpressionButton::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	const auto contentSize = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->m_FillExpression));
	const auto contentMetricSize = SMetricSizeF(
		Graphical::GetMetricFromPixels(graphicContext.DpiX(), contentSize.width()),
		Graphical::GetMetricFromPixels(graphicContext.DpiY(), contentSize.height()));

	const auto calculatedWidth = contentMetricSize.CX() + textMarginLeft + textMarginRight;
	const auto calculatedHeight = textMarginTop + textMarginBottom + contentMetricSize.CY();
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CFillExpressionButton::SetBodyPosition(
	const SMetricRectangleF& allocatedArea,
	const CGraphicContext& graphicContext)
{
	const auto contentSize = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->m_FillExpression));
	const auto contentMetricSize = SMetricSizeF(
		Graphical::GetMetricFromPixels(graphicContext.DpiX(), contentSize.width()),
		Graphical::GetMetricFromPixels(graphicContext.DpiY(), contentSize.height()));

	this->SetBodyArea(allocatedArea);
	this->SetFillExpressionArea(
		SMetricRectangleF(
			allocatedArea.Left() + textMarginLeft + (contentMetricSize.CX() / 2),
			allocatedArea.Top() + textMarginTop + (contentMetricSize.CY() / 2),
			contentSize.width(),
			contentSize.height()));
}

void CFillExpressionButton::Paint(
	const CGraphicContext& graphicContext,
	const QPainter& painter)
{
	
}

