#ifndef CFLOORPLAN_H__
#define CFLOORPLAN_H__

#include <vector>

#include "SMetricSizeF.h"
#include "SMetricRectangleF.h"
#include "CMemoryRegionBlock.h"

class QFontMetrics;

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents a Linker-Script object.
    class CFloorPlan : public CDrawableObjectBase
    {
        DECLARE_READONLY_PROPERTY(SharedPtrVector<CMemoryRegionBlock>, MemoryRegions)

    public:
        /// @brief Default constructor.
        CFloorPlan(
            const SharedPtrVector<CMemoryRegionBlock>& memorySections) :
				m_MemoryRegions(memorySections)
        {}

    public:
        /// @copydoc CDrawableObjectBase::Paint
        void Paint(
            const CGraphicContext& graphicContext,
            QPainter& painter) override;

        /// @brief Calculate the minimum amount of space this region would need.
        SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const;

        /// @brief Sets the geometry of the floor-plan. We'll cascade this down to
        ///        child memory sections.
        void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) const;
    };
}

#endif // CFLOORPLAN_H__
