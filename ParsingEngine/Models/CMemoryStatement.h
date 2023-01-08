#ifndef CMEMORY_STATEMENT_H__
#define CMEMORY_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /*
    MEMORY
    {
        rom (rx)  : ORIGIN = 0, LENGTH = 256K
        ram (!rx) : org = 0x40000000, l = 4M
    }

    ‘R’
    Read-only section

    ‘W’
    Read/write section

    ‘X’
    Executable section

    ‘A’
    Allocatable section

    ‘I’
    Initialized section

    ‘L’
    Same as ‘I’

    ‘!’
    Invert the sense of any of the attributes that follow
    */

    /// @brief Represents a single entry in the "MEMORIES" region
    class CMemoryStatement : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatement(std::vector<CRawEntry>&& composingRawElements,
                                  std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(composingRawElements), std::move(violations))
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryStatement;
        }
    };
}

#endif
