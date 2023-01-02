#ifndef CPROCEDURE_CALL_STATEMENT_H__
#define CPROCEDURE_CALL_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a procedural call
    class CProcedureCall : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of
        explicit CProcedureCall(std::vector<CRawEntry>&& rawElements, 
                                std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProcedureCall;
        }
    };
}

#endif
