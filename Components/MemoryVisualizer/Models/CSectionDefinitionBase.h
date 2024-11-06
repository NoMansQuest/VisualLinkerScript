#ifndef CSECTION_DEFINITION_BASE_H__
#define CSECTION_DEFINITION_BASE_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CContentBase.h"
#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Statement.
    class CSectionDefinitionBase : public CContentBase
    {    
	    DECLARE_STANDARD_PROPERTY(std::string, Title)
	    DECLARE_STANDARD_PROPERTY(std::string, FillExpression)
        DECLARE_STANDARD_PROPERTY(std::vector<std::string>, ProgramHeaders)

    protected:
        ~CSectionDefinitionBase() = default;

    public:
        /// @brief Default constructor
        CSectionDefinitionBase(bool isExternal)
            : CContentBase(true, isExternal)
        {}
    };
}

#endif // CMEMORYSTATEMENT_H