#include "CMemorySection.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;


SMetricSizeF CMemorySection::CalculateDesiredSize(const QFontMetrics& fontMetrics)
{
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

void CMemorySection::SetGeometry(SMetricRectangleF allocatedArea)
{
	
}
