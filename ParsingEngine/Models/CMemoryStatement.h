#ifndef CMEMORY_STATEMENT_H__
#define CMEMORY_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CMemoryStatement : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CMemoryStatement(std::vector<CRawEntry>&&  composingRawElements, 
                                  std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(composingRawElements, violations)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryStatement;
        }
    }
}

#endif