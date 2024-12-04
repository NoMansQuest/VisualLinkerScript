#include "CFillExpressionButton.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double textMarginLeft = 1;
constexpr double textMarginRight = 1;
constexpr double textMarginBottom = 1;
constexpr double textMarginTop = 1;

SMetricSizeF CFillExpressionButton::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{
	auto contentSize = fontMetricsSmall.boundingRect(QString::fromStdString(this->m_FillExpression));
	auto contentMetricSize = SMetricSizeF(
		Graphical::GetMetricFromPixels(dpiX, contentSize.width()),
		Graphical::GetMetricFromPixels(dpiY, contentSize.height()));

	auto calculatedWidth = contentMetricSize.CX() + textMarginLeft + textMarginRight;
	auto calculatedHeight = textMarginTop + textMarginBottom + contentMetricSize.CY();
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CFillExpressionButton::SetBodyPosition(
	const SMetricRectangleF& allocatedArea,
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge)
{
	auto contentSize = fontMetricsSmall.boundingRect(QString::fromStdString(this->m_FillExpression));
	auto contentMetricSize = SMetricSizeF(
		Graphical::GetMetricFromPixels(dpiX, contentSize.width()),
		Graphical::GetMetricFromPixels(dpiY, contentSize.height()));

	this->SetBodyArea(allocatedArea);
	this->SetFillExpressionArea(
		SMetricRectangleF(
			allocatedArea.Left() + textMarginLeft + (contentMetricSize.CX() / 2),
			allocatedArea.Top() + textMarginTop + (contentMetricSize.CY() / 2),
			contentSize.width(),
			contentSize.height()));
}

void CFillExpressionButton::Paint(const QPainter& painter)
{
	
}

