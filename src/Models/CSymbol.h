#ifndef CSECONDARY_SYMBOL_H__
#define CSECONDARY_SYMBOL_H__

#include <vector>
#include "Models/CParsedContentBase.h"
#include "Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a symbol which is R-Value or L-Value expression and may be preceded by an arithmetic sign.
    class CSymbol : public CParsedContentBase
    { 
    private:
        CRawEntry m_symbol;

    public:        
        /// @brief Default constructor, accessible to inheritors only
        explicit CSymbol(const CRawEntry& symbol,
                         const std::vector<CRawEntry>& rawElements, 
                         const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
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

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();
        }
    };
}

#endif
