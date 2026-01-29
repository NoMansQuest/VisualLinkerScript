#include "Components/MemoryVisualizer/Composition/CAddressedRegion.h"
#include "Components/MemoryVisualizer/ColorResources.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double addressStartConnectingLineLengthMm = 1;
constexpr double addressStartTextToLineHSpaceMm = 5;
constexpr double sizeMarkerFirstLineLengthMm = 5;
constexpr double sizeMarkerSecondLineLengthMm = 3;
constexpr double sizeMarkerTextToLineMargin = 1;

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
			Qt::AlignRight | Qt::AlignVCenter,
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
			Qt::AlignRight | Qt::AlignVCenter,
			QString::fromStdString(this->AddressEndText()));

		painter.setPen(QColor::fromRgba(Colors::AddressMarkerLineColor));
		painter.drawLine(
			this->AddressEndConnctingLine().ToStartQPointF(graphicContext),
			this->AddressEndConnctingLine().ToEndQPointF(graphicContext));
	}

	// Draw the 'Size' marker (on the right side)
	// In case of memory regions, their size is always known
	if (this->MemorySizeKnown())
	{
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
			this->SizeMarkerVerticalLine().ToStartQPointF(graphicContext),
			this->SizeMarkerVerticalLine().ToEndQPointF(graphicContext));

		painter.drawLine(
			this->SizeMarkerCenterConnector().ToStartQPointF(graphicContext),
			this->SizeMarkerCenterConnector().ToEndQPointF(graphicContext));
	}
}


void CAddressedRegion::SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext)
{
	// Set address start and top markers		
	auto addressStartTextWidthInMm = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressStartText())));
	auto addressEndTextWidthInMm = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->AddressEndText())));
	auto sizeMarkerTextWidthInMm = Graphical::GetMetricFromPixels(graphicContext.DpiX(), graphicContext.FontMetricsSmall().horizontalAdvance(QString::fromStdString(this->SizeMarkerText())));

	auto smallFontHeightHalfInMm = Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()) / 2);
	auto smallFontHeightInMm = Graphical::GetMetricFromPixels(graphicContext.DpiY(), static_cast<double>(graphicContext.FontMetricsSmall().height()));

	this->SetAddressStartTextArea(
		SMetricRectangleF(
			this->BodyArea().Left() - addressStartConnectingLineLengthMm - addressStartTextWidthInMm - addressStartTextToLineHSpaceMm,
			this->BodyArea().Top() - smallFontHeightHalfInMm,
			addressStartTextWidthInMm,
			smallFontHeightInMm));

	this->SetAddressStartConnectingLine(
		SLineF(
			this->AddressStartTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Top(),
			this->BodyArea().Left(),
			this->BodyArea().Top()));

	this->SetAddressEndTextArea(
		SMetricRectangleF(
			this->BodyArea().Left() - addressStartConnectingLineLengthMm - addressEndTextWidthInMm - addressStartTextToLineHSpaceMm,
			this->BodyArea().Bottom() - smallFontHeightHalfInMm,
			addressEndTextWidthInMm,
			smallFontHeightInMm));

	this->SetAddressEndConnctingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Bottom(),
			this->BodyArea().Left(),
			this->BodyArea().Bottom()));

	// Set size marker
	this->SetSizeMarkerTextArea(
		SMetricRectangleF(
			this->BodyArea().Right() + this->SizeMarkerDepth() + sizeMarkerSecondLineLengthMm + sizeMarkerTextToLineMargin,
			this->BodyArea().Center().Y() - smallFontHeightHalfInMm,
			sizeMarkerTextWidthInMm,
			smallFontHeightInMm));

	this->SetSizeMarkerUpperConnector(
		SLineF(
			this->BodyArea().Right(),
			this->BodyArea().Top(),
			this->BodyArea().Right() + this->SizeMarkerDepth(),
			this->BodyArea().Top()));

	this->SetSizeMarkerLowerConnector(
		SLineF(
			this->BodyArea().Right(),
			this->BodyArea().Bottom(),
			this->BodyArea().Right() + this->SizeMarkerDepth(),
			this->BodyArea().Bottom()));

	this->SetSizeMarkerVerticalLine(
		SLineF(
			this->BodyArea().Right() + this->SizeMarkerDepth(),
			this->BodyArea().Top(),
			this->BodyArea().Right() + this->SizeMarkerDepth(),
			this->BodyArea().Bottom()));

	this->SetSizeMarkerCenterConnector(
		SLineF(
			this->BodyArea().Right() + this->SizeMarkerDepth(),
			this->BodyArea().Center().Y(),
			this->BodyArea().Right() + this->SizeMarkerDepth() + sizeMarkerSecondLineLengthMm,
			this->BodyArea().Center().Y()));
}
