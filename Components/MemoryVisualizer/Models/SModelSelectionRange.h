#ifndef SMODEL_SELECTION_RANGE_H__
#define SMODEL_SELECTION_RANGE_H__

#include "../../../Helpers.h"

/// @brief Points to a start and end position in the linker-script text that corresponds to the
///        UI item displayed to the user. The text editor should select this range when the parent
///        object is activated.
struct SModelSelectionRange
{
    DECLARE_READONLY_PROPERTY(uint32_t, ModelRangeStart)
    DECLARE_READONLY_PROPERTY(uint32_t, ModelRangeLength)

    /// @brief Parameterized constructor;
    SModelSelectionRange(
        const uint32_t modelRangeStart,
        const uint32_t modelRangeLength)
    {
        this->m_ModelRangeLength = modelRangeLength;
        this->m_ModelRangeStart = modelRangeStart;
    }
};


#endif // SMODEL_SELECTION_RANGE_H__
