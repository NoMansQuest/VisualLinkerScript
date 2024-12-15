#include "CAddressedRegion.h"
#include "../ColorResources.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double addressStartConnectingLineLengthMm = 10;
constexpr double addressStartTextToLineHSpaceMm = 5;
constexpr double sizeMarkerFirstLineLengthMm = 5;
constexpr double sizeMarkerSecondLineLengthMm = 5;

void CAddressedRegion::PaintAddressedRegion(
    const CGraphicContext& graphicContext,
    QPainter& painter,
    const QPen& bodyBorderPen,
    const QBrush& bodyFillBrush) const
{
	// Draw surrounding rectangle
	painter.setPen(bodyBorderPen);
	painter.fillRect(this->BodyArea().ConvertToQRect(graphicContext), bodyFillBrush);
	painter.drawRect(this->BodyArea().ConvertToQRect(graphicContext));

	// Draw Start and Stop address markers
	if (this->StartAddressKnown())
	{
		painter.setFont(graphicContext.FontSmall());
		painter.setPen(QColor::fromRgba(Colors::AddressMarkerTextColor));
		painter.drawText(
			this->AddressStartTextArea().ConvertToQRect(graphicContext),
			Qt::AlignHCenter | Qt::AlignVCenter,
			QString::fromStdString(this->AddressStartText()));

		painter.setPen(QColor::fromRgba(Colors::AddressMarkerLineColor));
		painter.drawLine(
			this->AddressStartConnectingLine().ToStartQPointF(graphicContext),
			this->AddressStartConnectingLine().ToEndQPointF(graphicContext));
	}

	// Draw End and Stop address markers
	if (this->EndAddressKnown())
	{
		painter.setFont(graphicContext.FontSmall());
		painter.setPen(QColor::fromRgba(Colors::AddressMarkerTextColor));
		painter.drawText(
			this->AddressEndTextArea().ConvertToQRect(graphicContext),
			Qt::AlignHCenter | Qt::AlignVCenter,
			QString::fromStdString(this->AddressStartText()));

		painter.setPen(QColor::fromRgba(Colors::AddressMarkerLineColor));
		painter.drawLine(
			this->AddressEndConnctingLine().ToStartQPointF(graphicContext),
			this->AddressEndConnctingLine().ToEndQPointF(graphicContext));
	}

	// Draw the 'Size' marker (on the right side)
	// In case of memory regions, their size is always known
	painter.setFont(graphicContext.FontSmall());
	painter.setPen(QColor::fromRgba(Colors::SizeMarkerTextColor));
	painter.drawText(
		this->SizeMarkerTextArea().ConvertToQRect(graphicContext),
		Qt::AlignHCenter | Qt::AlignVCenter,
		QString::fromStdString(this->SizeMarkerText()));

	painter.setPen(QColor::fromRgba(Colors::SizeMarkerLineColor));
	painter.drawLine(
		this->SizeMarkerLowerConnector().ToStartQPointF(graphicContext),
		this->SizeMarkerLowerConnector().ToEndQPointF(graphicContext));

	painter.drawLine(
		this->SizeMarkerUpperConnector().ToStartQPointF(graphicContext),
		this->SizeMarkerUpperConnector().ToEndQPointF(graphicContext));

	painter.drawLine(
		this->SizeMarkerCenterConnector().ToStartQPointF(graphicContext),
		this->SizeMarkerCenterConnector().ToEndQPointF(graphicContext));
}


void CAddressedRegion::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	// Set address start and top markers		
	auto addressStartTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressStartText())));
	auto addressEndTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressEndText())));
	auto sizeMarkerTextWidth = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->SizeMarkerText())));

	auto smallFontHeightHalf = static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2;
	auto smallFontHeight = static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2;

	this->SetAddressStartTextArea(
		SMetricRectangleF(
			this->BodyArea().Left() - addressStartConnectingLineLengthMm - addressStartTextWidth - addressStartTextToLineHSpaceMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			addressStartTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeight)));

	this->SetAddressStartConnectingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf)));

	this->SetAddressEndTextArea(
		SMetricRectangleF(
			this->BodyArea().Left() - addressStartConnectingLineLengthMm - addressEndTextWidth - addressStartTextToLineHSpaceMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2),
			addressEndTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeight)));

	this->SetAddressEndConnctingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressEndTextWidth,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf)));

	// Set size marker
	this->SetSizeMarkerTextArea(
		SMetricRectangleF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeightHalf),
			sizeMarkerTextWidth,
			Graphical::GetMetricFromPixels(graphicContext.DpiY(), smallFontHeight)));

	this->SetSizeMarkerUpperConnector(
		SLineF(
			this->BodyArea().Right(),
			this->BodyArea().Top(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top()));

	this->SetSizeMarkerUpperConnector(
		SLineF(
			this->BodyArea().Right(),
			this->BodyArea().Bottom(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom()));

	this->SetSizeMarkerVerticalLine(
		SLineF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom()));

	this->SetSizeMarkerCenterConnector(
		SLineF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Center().Y(),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			this->BodyArea().Center().Y()));
}
