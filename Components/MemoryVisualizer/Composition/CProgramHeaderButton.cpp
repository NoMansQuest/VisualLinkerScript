#include "CProgramHeaderButton.h"

#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double textMarginLeft = 1;
constexpr double textMarginRight = 1;
constexpr double textMarginBottom = 1;
constexpr double textMarginTop = 1;

SMetricSizeF CProgramHeaderButton::CalculateBodySize(const CGraphicContext& graphicContext) const
{	
	auto contentSize = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->m_ProgramHeaderText));
	auto contentMetricSize = SMetricSizeF(
		Graphical::GetMetricFromPixels(graphicContext.DpiX(), contentSize.width()),
		Graphical::GetMetricFromPixels(graphicContext.DpiY(), contentSize.height()));

	auto calculatedWidth = contentMetricSize.CX() + textMarginLeft + textMarginRight;
	auto calculatedHeight = textMarginTop + textMarginBottom + contentMetricSize.CY();
	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CProgramHeaderButton::SetBodyPosition(
	const SMetricRectangleF& allocatedArea,
	const CGraphicContext& graphicContext)
{
	auto contentSize = graphicContext.FontMetricsSmall().boundingRect(QString::fromStdString(this->m_ProgramHeaderText));
	auto contentMetricSize = SMetricSizeF(
		Graphical::GetMetricFromPixels(graphicContext.DpiX(), contentSize.width()),
		Graphical::GetMetricFromPixels(graphicContext.DpiY(), contentSize.height()));

	this->SetBodyArea(allocatedArea);
	this->SetProgramHeaderTextArea(
		SMetricRectangleF(
			allocatedArea.Left() + textMarginLeft + (contentMetricSize.CX() / 2),
			allocatedArea.Top() + textMarginTop + (contentMetricSize.CY() / 2),
			contentSize.width(),
			contentSize.height()));
}

void CProgramHeaderButton::Paint(const CGraphicContext& graphicContext, QPainter& painter)
{
	QRgb borderColor = qRgba(0, 0, 0, 0);
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
	const auto borderPen = QPen(QColor::fromRgb(borderColor), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
	const auto fillBrush = QBrush(QColor::fromRgba(backgroundColor), Qt::SolidPattern);
	painter.setPen(borderPen);
	painter.fillRect(this->BodyArea().ConvertToQRect(graphicContext), fillBrush);
	painter.drawRect(this->BodyArea().ConvertToQRect(graphicContext));

	// Draw section name
	const auto textPen = QPen(QColor::fromRgb(textColor));
	painter.setPen(textPen);
	painter.setFont(graphicContext.FontSmallBold());
	painter.drawText(this->BodyArea().ConvertToQRect(graphicContext), Qt::AlignHCenter | Qt::AlignVCenter, QString::fromStdString(this->ProgramHeaderText()));
}