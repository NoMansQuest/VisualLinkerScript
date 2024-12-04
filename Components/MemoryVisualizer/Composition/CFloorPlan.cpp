#include "CFloorPlan.h"
#include <QFont>
#include <QFontMetrics>
#include "CMemorySection.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double SpaceBetweenMemoryRegionsMm = 10;
constexpr double Max(const double a, const double b) { return a > b ? a : b;  }

SMetricSizeF CFloorPlan::CalculateBodySize(
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{
	if (this->m_MemorySections.empty())
	{
		return { 0, 0 };
	}

	double maxWidthObserved = 0;
	double calculatedHeight = 0;

	for (const auto& memorySection : this->m_MemorySections)
	{
		auto calculatedSize = memorySection->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		maxWidthObserved = Max(maxWidthObserved, calculatedSize.CX());
		calculatedHeight += calculatedSize.CY() + SpaceBetweenMemoryRegionsMm;
	}

	return SMetricSizeF(maxWidthObserved, calculatedHeight);
}

void CFloorPlan::SetBodyPosition(
	const SMetricRectangleF& allocatedArea,
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{
	double currentPositionHolder = allocatedArea.Top();
	for (const auto& memorySection : this->m_MemorySections)
	{
		auto calculatedSize = memorySection->CalculateBodySize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		SMetricRectangleF area(allocatedArea.Left(), currentPositionHolder, calculatedSize.CX(), calculatedSize.CY());
		memorySection->SetBodyPosition(area, dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		currentPositionHolder += area.Height() + SpaceBetweenMemoryRegionsMm;
	}
}

void CFloorPlan::Paint(const QPainter& painter)
{

}