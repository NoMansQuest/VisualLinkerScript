#ifndef CEXPRESSION_H__
#define CEXPRESSION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a compound expression, composed of multiple symbols (and/or sub expressions).
    class CExpression: public CLinkerScriptContentBase
    {
    private:
        std::vector<CRawEntry> m_assignmentOperator;        
        std::vector<CLinkerScriptContentBase> m_composition;        

    public:
        /// @brief Parameterized constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CExpression(CRawEntry openingParenthesis,
                             CRawEntry closingParenthesis,
                             std::vector<CLinkerScriptContentBase>&& composition,
                             std::vector<CRawEntry>&& rawElements, 
                             std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_composition(composition)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Expression;
        }    

        /// @brief Gets the "Composition" 
        std::vector<CLinkerScriptContentBase>& Composition() const
        {
            return this->m_composition;
        }
    };
}

#endif
