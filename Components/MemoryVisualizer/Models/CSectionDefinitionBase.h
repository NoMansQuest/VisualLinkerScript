#ifndef CSECTION_DEFINITION_BASE_H__
#define CSECTION_DEFINITION_BASE_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CAddressedContent.h"
#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Statement.
    class CSectionDefinitionBase : public CAddressedContent
    {
        DECLARE_READONLY_PROPERTY(std::string, Title)
	    DECLARE_STANDARD_PROPERTY(std::string, FillExpression)
        DECLARE_STANDARD_PROPERTY(std::vector<std::string>, ProgramHeaders)

    protected:
        ~CSectionDefinitionBase() = default;

    public:
        /// @brief Default constructor
        CSectionDefinitionBase(
            std::string title, 
            std::string fillExpression, 
            const std::vector<std::string>& programHeaders)
		    : m_Title(std::move(title)),
		      m_FillExpression(std::move(fillExpression)),
		      m_ProgramHeaders(programHeaders)
	    {
	    }
    };
}

#endif // CMEMORYSTATEMENT_H