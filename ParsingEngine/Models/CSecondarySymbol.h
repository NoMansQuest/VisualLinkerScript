#ifndef CSECONDARY_SYMBOL_H__
#define CSECONDARY_SYMBOL_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a symbol which is positioned a R-Value and is preceded by an arithmetic sign.
    class CSecondarySymbol : public CLinkerScriptContentBase
    { 
    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SecondarySymbol;
        }    

    private:
        bool m_hasPrecedingOperator;
        CRawEntry m_arithmeticOperator;
        CRawEntry m_symbol;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSecondarySymbol(CRawEntry precedingOperator,
                         CRawEntry symbol,
                         std::vector<CRawEntry>&& rawElements, 
                         std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_arithmeticOperator(arithmeticOperator),
              m_symbol(symbol)
        {}   

    public:
        /// @brief Gets the arithmetic operator        
        CRawEntry& ArithmeticOperator() const
        {
            return this->m_arithmeticOperator;
        }

        /// @brief Gets the symbol itself
        CRawEntry& Symbol() const
        {
            return this->m_symbol;
        }
    };
}

#endif
