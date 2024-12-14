#include "CFloorPlan.h"
#include <QFont>
#include <QFontMetrics>
#include "CMemoryRegion.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

constexpr double SpaceBetweenMemoryRegionsMm = 4;
constexpr double Max(const double a, const double b) { return a > b ? a : b;  }

SMetricSizeF CFloorPlan::CalculateBodySize(const CGraphicContext& graphicContext) const
{
	if (this->m_MemoryRegions.empty())
	{
		return { 0, 0 };
	}

	double maxWidthObserved = 0;
	double calculatedHeight = 0;

	for (const auto& memorySection : this->m_MemoryRegions)
	{
		auto calculatedSize = memorySection->CalculateBodySize(graphicContext);
		maxWidthObserved = Max(maxWidthObserved, calculatedSize.CX());
		calculatedHeight += calculatedSize.CY() + SpaceBetweenMemoryRegionsMm;
	}

	return { maxWidthObserved, calculatedHeight };
}

void CFloorPlan::SetBodyPosition(
	const SMetricRectangleF& allocatedArea,
	const CGraphicContext& graphicContext) const
{
	double currentPositionHolder = allocatedArea.Top();
	for (const auto& memorySection : this->m_MemoryRegions)
	{
		auto calculatedSize = memorySection->CalculateBodySize(graphicContext);
		SMetricRectangleF area(allocatedArea.Left(), currentPositionHolder, calculatedSize.CX(), calculatedSize.CY());
		memorySection->SetBodyPosition(area, graphicContext);
		currentPositionHolder += area.Height() + SpaceBetweenMemoryRegionsMm;
	}
}

void CFloorPlan::Paint(
	const CGraphicContext& graphicContext, 
	QPainter& painter)
{
	for (const auto& memoryRegion : this->MemoryRegions())
	{
		memoryRegion->Paint(graphicContext, painter);
	}
}