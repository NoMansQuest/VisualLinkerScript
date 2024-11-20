#ifndef CGENERIC_SECTION_DEFINITION_H_
#define CGENERIC_SECTION_DEFINITION_H_

#include "CSectionDefinitionBase.h"
#include "CSectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Statement.
    class CSectionStatement : public CSectionDefinitionBase
    {
        DECLARE_READONLY_PROPERTY(SharedPtrVector<CSectionOutput>, ChildOutputs)

    protected:
        ~CSectionStatement() = default;

    public:
        /// @brief Default constructor
        CSectionStatement(
				std::string title, 
				std::string fillExpression, 
	            const std::vector<std::string>& programHeaders, 
	            const SharedPtrVector<CSectionOutput>& childOutputs) :
    		CSectionDefinitionBase(std::move(title), std::move(fillExpression), programHeaders),
    		m_ChildOutputs(childOutputs)
        {}
    };
}

#endif // CGENERIC_SECTION_DEFINITION_H_
