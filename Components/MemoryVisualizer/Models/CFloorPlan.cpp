#include "CFloorPlan.h"
#include <QFont>
#include <QFontMetrics>
#include "CMemorySection.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

#define SPACE_BETWEEN_MEMORY_REGIONS_MM 10
#define MAX(a, b) ((a > b) ? a : b)

SMetricSizeF CFloorPlan::CalculateDesiredSize(const QFontMetrics& fontMetrics) const
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

	for (const auto memorySection& : this->m_MemorySections)
	{
		auto calculatedSize = memorySection->CalculateDesiredSize(fontMetrics);
		maxWidthObserved = MAX(maxWidthObserved, calculatedSize.CX());
		maxHeightObserved = MAX(maxHeightObserved, calculatedSize.CY());
	}

	return SMetricSizeF(
		maxWidthObserved,
		((this->m_MemorySections.size() - 1) * SPACE_BETWEEN_MEMORY_REGIONS_MM) + maxHeightObserved);		
}

void CFloorPlan::SetGeometry(SMetricRectangleF allocatedArea)
{
	// No action undertaken. The floor plan itself is not displayed on the screen.
}