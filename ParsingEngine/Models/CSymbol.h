#ifndef CSECONDARY_SYMBOL_H__
#define CSECONDARY_SYMBOL_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a symbol which is R-Value or L-Value expression and may be preceded by an arithmetic sign.
    class CSymbol : public CLinkerScriptContentBase
    { 
    private:
        CRawEntry m_symbol;

    public:        
        /// @brief Default constructor, accessible to inheritors only
        explicit CSymbol(CRawEntry symbol,
                         std::vector<CRawEntry>&& rawElements, 
                         std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_symbol(symbol)
        {}   

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SecondarySymbol;
        }    

        /// @brief Gets the symbol itself
        const CRawEntry& Symbol()
        {
            return this->m_symbol;
        }
    };
}

#endif
