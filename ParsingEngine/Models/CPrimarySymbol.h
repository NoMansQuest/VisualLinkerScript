#ifndef CPRIMARY_SYMBOL_H__
#define CPRIMARY_SYMBOL_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CPrimarySymbol : public CLinkerScriptContentBase
    { 
    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::PrimarySymbol;
        }    

    private:
        CRawEntry m_symbol;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param symbol The raw-entry containing the symbol
        /// @param rawElements A list of object this element is comprised of.
        explicit CSymbol(CRawEntry symbol,
                         std::vector<CRawEntry>&& rawElements, 
                         std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_symbol(symbol)
        {}   

        /// @brief Gets the symbol itself
        CRawEntry& Symbol() const
        {
            return this->m_symbol;
        }     
    };
}

#endif
