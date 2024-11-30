#include "CFloorPlan.h"
#include <QFont>
#include <QFontMetrics>
#include "CMemorySection.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

constexpr double SpaceBetweenMemoryRegionsMm = 10;
constexpr double Max(const double a, const double b) { return a > b ? a : b;  }

SMetricSizeF CFloorPlan::CalculateDesiredSize(
	const QFontMetrics& fontMetricsSmall,
	const QFontMetrics& fontMetricsLarge) const
{
	/*
	QFont font("Tahoma, DejaVu Sans", 12); 
	font.setFamily("Arial, Helvetica, sans-serif"); // Specify primary and fallback fonts
	QFontMetrics fontMetrics(font);
	*/

	if (this->m_MemorySections.empty())
	{
		return { 0, 0 };
	}

	double maxWidthObserved = 0;
	double maxHeightObserved = 0;

	for (const auto memorySection : this->m_MemorySections)
	{
		auto calculatedSize = memorySection->CalculateDesiredSize(fontMetricsSmall, fontMetricsLarge);
		maxWidthObserved = Max(maxWidthObserved, calculatedSize.CX());
		maxHeightObserved = Max(maxHeightObserved, calculatedSize.CY());
	}

	return SMetricSizeF(
		maxWidthObserved,
		((this->m_MemorySections.size() - 1) * SpaceBetweenMemoryRegionsMm) + maxHeightObserved);		
}
