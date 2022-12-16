#ifndef CSECTION_OUTPUT_STATEMENT_H__
#define CSECTION_OUTPUT_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOutputStatement : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionOutputStatement(std::vector<CRawEntry>&& rawElements, 
                                         std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOutputStatement;
        }
    }
}

#endif