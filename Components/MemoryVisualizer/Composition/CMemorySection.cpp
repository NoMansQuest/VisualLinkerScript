#include "CMemorySection.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double textToMemoryRegionVSpaceMm = 2;
constexpr double titleToSizeTextHSpaceMm = 2;
constexpr double memoryRegionBorderMm = 0.5;
constexpr double sectionToSectionVSpaceMm = 4;
constexpr double addressStartConnectingLineLengthMm = 10;
constexpr double addrssStartTextToLineHSpaceMm = 5;
constexpr double sizeMarkerFirstLineLengthMm = 40;
constexpr double sizeMarkerSecondLineLengthMm = 5;

SMetricSizeF CMemorySection::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge )
{	
	// Constants
	double minimumMemoryBoxHeight = 10 + 2; // 14mm minimum memory box height. 4mm Text Label + 2mm Space + 0.5mm Border + 7mm Empty Space + 0.5mm border

	// Top memory label
	auto topMemoryLabel = Graphical::GetTextWidthInPixels("Memory Size: 16382 KB", fontMetricsLarge);	

	// Left size address text
	auto leftAddressTextSize = Graphical::GetTextWidthInPixels("0x0000000000000000", fontMetricsSmall);
	double leftAddressLine = addressStartConnectingLineLengthMm; // 10mm = 1mm Space + 8mm Line + 1mm Space
	double leftSizeTotal = leftAddressTextSize + leftAddressLine;

	// Right side memory size lines
	double rightSizeLine1 = sizeMarkerFirstLineLengthMm; 
	double rightSizeLine2 = sizeMarkerSecondLineLengthMm; 
	double rightSizeText = Graphical::GetTextWidthInPixels("4096 Bytes", fontMetricsSmall);
	double rightSizeTotal = rightSizeLine1 + rightSizeLine2 + rightSizeText;
		
	double calculatedWidth = leftSizeTotal + rightSizeTotal + (topMemoryLabel * 1.5);
	double calculatedHeight = minimumMemoryBoxHeight;

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
			calculatedWidth = std::max(calculatedWidth, childDesiredSize.CX());
			calculatedHeight += childDesiredSize.CY() + sectionToSectionVSpaceMm;
		}
	}

	return SMetricSizeF(calculatedWidth, calculatedHeight);
}

void CMemorySection::SetBodyPosition(
	const SMetricRectangleF allocatedArea,
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) 
{
	auto calculatedDesiredSize = this->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
	auto currentYHolder = allocatedArea.Top();

	auto topMemoryLabelRect = 
		SMetricRectangleF(fontMetricsLarge.boundingRect(QString::fromStdString(this->m_Title)), dpiX, dpiY)
		.Offset(allocatedArea.Left(), allocatedArea.Top());

	auto topMemorySizeRect = 
		SMetricRectangleF(fontMetricsLarge.boundingRect(QString::fromStdString(this->m_MemorySizeText)), dpiX, dpiY)
		.Offset(allocatedArea.Left() + topMemoryLabelRect.Width() + titleToSizeTextHSpaceMm, allocatedArea.Top());

	currentYHolder += fontMetricsLarge.height() + textToMemoryRegionVSpaceMm;
	double memoryRegionTop = currentYHolder;

	if (!this->m_ChildContent.empty())
	{
		for (const auto& child : this->m_ChildContent)
		{
			auto childDesiredSize = child->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
			child->SetBodyPosition(
				SMetricRectangleF(allocatedArea.Left(), currentYHolder, childDesiredSize.CX(), childDesiredSize.CY()),
				dpiX, 
				dpiY, 
				fontMetricsSmall, 
				fontMetricsLarge);

			currentYHolder += childDesiredSize.CY() + sectionToSectionVSpaceMm;
		}
	}

	double memoryRegionBottom = currentYHolder;
	double memoryAreaHeight = memoryRegionBottom - memoryRegionTop;

	this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), memoryRegionTop, calculatedDesiredSize.CX(), memoryAreaHeight));

	// Set address start and top markers		
	auto addressStartTextWidth = Graphical::GetMetricFromPixels(dpiX, fontMetricsSmall.horizontalAdvance(QString::fromStdString(this->AddressStartText())));
	auto addressEndTextWidth = Graphical::GetMetricFromPixels(dpiX, fontMetricsSmall.horizontalAdvance(QString::fromStdString(this->AddressEndText())));
	auto sizeMarkerTextWidth = Graphical::GetMetricFromPixels(dpiX, fontMetricsSmall.horizontalAdvance(QString::fromStdString(this->SizeMarkerText())));

	this->SetAddressStartTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressStartTextWidth - addrssStartTextToLineHSpaceMm,
			memoryRegionTop - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2),
			addressStartTextWidth,
			Graphical::GetMetricFromPixels(dpiY, fontMetricsSmall.height())));

	this->SetAddressStartConnectingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressStartConnectingLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Top() - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2)));

	this->SetAddressEndTextArea(
		SMetricRectangleF(
			allocatedArea.Left() - addressStartConnectingLineLengthMm - addressEndTextWidth - addrssStartTextToLineHSpaceMm,
			memoryRegionTop - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2),
			addressEndTextWidth,
			Graphical::GetMetricFromPixels(dpiY, fontMetricsSmall.height())));

	this->SetAddressEndConnctingLine(
		SLineF(
			this->AddressEndTextArea().Right() + addressEndTextWidth,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2),
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm,
			this->BodyArea().Bottom() - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2)));
	
	// Set size marker
	this->SetSizeMarkerTextArea(
		SMetricRectangleF(
			this->BodyArea().Right() + sizeMarkerFirstLineLengthMm + sizeMarkerSecondLineLengthMm,
			memoryRegionTop - Graphical::GetMetricFromPixels(dpiY, static_cast<double>(fontMetricsSmall.height()) / 2),
			sizeMarkerTextWidth,
			Graphical::GetMetricFromPixels(dpiY, fontMetricsSmall.height())));

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

void CMemorySection::Paint(const QPainter& painter)
{

}