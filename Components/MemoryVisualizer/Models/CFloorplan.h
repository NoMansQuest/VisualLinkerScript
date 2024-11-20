#ifndef CFLOORPLAN_H__
#define CFLOORPLAN_H__

#include <vector>

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    class CMemorySection;

    /// @brief Represents a Linker-Script object.
    class CFloorPlan
    {
        DECLARE_READONLY_PROPERTY(SharedPtrVector<CMemorySection>, MemorySections)

    public:
        /// @brief Default constructor.
        CFloorPlan(const SharedPtrVector<CMemorySection>& memorySections)
    		: m_MemorySections(memorySections)
        {}

    };
}

#endif // CFLOORPLAN_H__
