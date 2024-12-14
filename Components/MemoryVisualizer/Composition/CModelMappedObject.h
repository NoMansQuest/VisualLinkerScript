#ifndef CMODEL_MAPPED_OBJECT_H__
#define CMODEL_MAPPED_OBJECT_H__

#include "../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents an object that would correspond to a specific text found in the Linker script.
    class CModelMappedObject
    {
        DECLARE_READONLY_PROPERTY(uint32_t, InLinkerScriptStartPosition)
        DECLARE_READONLY_PROPERTY(uint32_t, InLinkerScriptLength)

        /// @brief Default constructor
        CModelMappedObject(const uint32_t inLinkerScriptStartPosition, const uint32_t inLinkerScriptLength) :
            m_InLinkerScriptStartPosition(inLinkerScriptStartPosition),
            m_InLinkerScriptLength(inLinkerScriptLength)
        {}
    };
}
#endif // CMODEL_MAPPED_OBJECT_H__
