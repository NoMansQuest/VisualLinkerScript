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

        /// @brief Default constructor
        CSectionStatement(
				const std::string& title, 
				const std::string& fillExpression, 
	            const std::vector<CProgramHeader>& programHeaders, 
	            const SharedPtrVector<CSectionOutput>& childOutputs, 
				const bool inModelStartPosition,
				const bool inModelLength,
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown) :
    		CSectionDefinitionBase(
                title, 
                fillExpression, 
                programHeaders, 
                inModelStartPosition, 
                inModelLength, 
                startAddressKnown, 
                endAddressKnown, 
                memorySizeKnown),
    		m_ChildOutputs(childOutputs)
        {}
    };
}

#endif // CGENERIC_SECTION_DEFINITION_H_
