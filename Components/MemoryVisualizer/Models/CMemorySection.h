#ifndef CMEMORY_SECTION_H__
#define CMEMORY_SECTION_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CContentBase.h"
#include "CSectionDefinitionBase.h"
#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Object.
    class CMemorySection : public CContentBase
    {
	    DECLARE_STANDARD_PROPERTY(std::string, Title)
	    DECLARE_READONLY_PROPERTY(SharedPtrVector<CSectionDefinitionBase>, ChildContent)

    protected:
        ~CMemorySection() = default;

    public:
        /// @brief Default constructor
        CMemorySection(bool isExternal)
            : CContentBase(false, isExternal)
        {}
    };
}

#endif // CMEMORY_SECTION_H__
