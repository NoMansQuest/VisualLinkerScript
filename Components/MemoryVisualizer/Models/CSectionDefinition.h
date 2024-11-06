#ifndef CGENERIC_SECTION_DEFINITION_H_
#define CGENERIC_SECTION_DEFINITION_H_

#include "CSectionDefinitionBase.h"
#include "CSectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Statement.
    class CGenericSectionDefinition : public CSectionDefinitionBase
    {
        DECLARE_READONLY_PROPERTY(SharedPtrVector<CSectionOutput>, ChildOutputs)

    protected:
        ~CGenericSectionDefinition() = default;

    public:
        /// @brief Default constructor
        CGenericSectionDefinition(bool isExternal)
            : CSectionDefinitionBase(isExternal)
        {}
    };
}

#endif // CGENERIC_SECTION_DEFINITION_H_
