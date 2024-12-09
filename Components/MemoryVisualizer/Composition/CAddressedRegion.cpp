#include "CAddressedRegion.h"
#include "../ColorResources.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

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


void CAddressedRegion::SetSizeAndAddressMarkerPosition()
{
	
}