#ifndef CFLOORPLAN_H__
#define CFLOORPLAN_H__

#include <vector>

#include "SMetricSizeF.h"
#include "CMemorySection.h"

struct SMetricRectangleF;
class QFontMetrics;

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Linker-Script object.
    class CFloorPlan
    {
        DECLARE_READONLY_PROPERTY(SharedPtrVector<CMemorySection>, MemorySections)

    public:
        /// @brief Default constructor.
        CFloorPlan(
            const SharedPtrVector<CMemorySection>& memorySections) :
    			m_MemorySections(memorySections)
        {}

    public:
        /// @brief Calculate the minimum amount of space this region would need.
        SMetricSizeF CalculateDesiredSize(
            const double dpiX,
            const double dpiY,
            const QFontMetrics& fontMetricsSmall,
            const QFontMetrics& fontMetricsLarge) const;

        /// @brief Sets the geometry of the floor-plan. We'll cascade this down to
        ///        child memory sections.
        void SetGeometry(
            const SMetricRectangleF& allocatedArea,
            const double dpiX,
            const double dpiY,
            const QFontMetrics& fontMetricsSmall,
            const QFontMetrics& fontMetricsLarge) const;
    };
}

#endif // CFLOORPLAN_H__
