#include "CFloorPlan.h"
#include <QFont>
#include <QFontMetrics>
#include "CMemorySection.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

constexpr double SpaceBetweenMemoryRegionsMm = 10;
constexpr double Max(const double a, const double b) { return a > b ? a : b;  }

SMetricSizeF CFloorPlan::CalculateDesiredSize(
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
		auto calculatedSize = memorySection->CalculateDesiredSize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		maxWidthObserved = Max(maxWidthObserved, calculatedSize.CX());
		calculatedHeight += calculatedSize.CY() + SpaceBetweenMemoryRegionsMm;
	}

	return SMetricSizeF(maxWidthObserved, calculatedHeight);
}

void CFloorPlan::SetGeometry(
	const SMetricRectangleF& allocatedArea,
	const double dpiX,
	const double dpiY,
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{
	double currentPositionHolder = allocatedArea.Top();
	for (const auto& memorySection : this->m_MemorySections)
	{
		auto calculatedSize = memorySection->CalculateDesiredSize(dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		SMetricRectangleF area(allocatedArea.Left(), currentPositionHolder, calculatedSize.CX(), calculatedSize.CY());
		memorySection->SetGeometry(area, dpiX, dpiY, fontMetricsSmall, fontMetricsLarge);
		currentPositionHolder += area.Height() + SpaceBetweenMemoryRegionsMm;
	}
}



