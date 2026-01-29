#include "Components/MemoryVisualizer/Composition/CFillExpressionButton.h"
#include "Components/MemoryVisualizer/Composition/CGraphicContext.h"
#include <QPainter>

#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double textMarginLeft = 1;
constexpr double textMarginRight = 1;
constexpr double textMarginBottom = 0.25;
constexpr double textMarginTop = 0.25;
constexpr double sizeMarkerFirstLineLengthMm = 0;

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

void CFillExpressionButton::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	QRgb borderColor = qRgba(0,0,0,0);
	QRgb backgroundColor = qRgba(0, 0, 0, 0);
	QRgb textColor = qRgba(0, 0, 0, 0);

	if (this->MouseDown())
	{
		borderColor = Colors::FillExpressionClickBorderColor;
		backgroundColor = Colors::FillExpressionClickBackground;
		textColor = Colors::FillExpressionTextClickForeColor;
	}
	else if (this->MouseHovering())
	{
		borderColor = Colors::FillExpressionHoverBorderColor;
		backgroundColor = Colors::FillExpressionHoverBackground;
		textColor = Colors::FillExpressionTextHoverForeColor;
	}
	else
	{
		borderColor = Colors::FillExpressionDefaultBorderColor;
		backgroundColor = Colors::FillExpressionDefaultBackground;
		textColor = Colors::FillExpressionTextDefaultForeColor;
	}

	// Draw button background
	const auto borderPen = QPen(QColor::fromRgba(borderColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(backgroundColor), Qt::SolidPattern);
	painter.setPen(borderPen);
	painter.fillRect(this->BodyArea().ConvertToQRect(graphicContext), fillBrush);
	painter.drawRect(this->BodyArea().ConvertToQRect(graphicContext));

	// Draw section name
	const auto textPen = QPen(QColor::fromRgba(textColor));
	painter.setPen(textPen);
	painter.setFont(graphicContext.FontSmallBold());
	painter.drawText(this->BodyArea().ConvertToQRect(graphicContext), Qt::AlignHCenter | Qt::AlignVCenter, QString::fromStdString(this->FillExpression()));
}

