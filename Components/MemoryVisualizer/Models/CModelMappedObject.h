#ifndef CMODEL_MAPPED_OBJECT_H__
#define CMODEL_MAPPED_OBJECT_H__

#include "../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents an object that would correspond to a specific text found in the Linker script.
    class CModelMappedObject
    {
        DECLARE_READONLY_PROPERTY(uint32_t, InModelStartPosition)
        DECLARE_READONLY_PROPERTY(uint32_t, InModelLength)

        /// @brief Default constructor
        CModelMappedObject(const uint32_t inModelStartPosition, const uint32_t inModelLength) :
            m_InModelStartPosition(inModelStartPosition),
            m_InModelLength(inModelLength)
        {}
    };
}
#endif // CMODEL_MAPPED_OBJECT_H__
