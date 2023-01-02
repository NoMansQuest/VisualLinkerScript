#ifndef CENCLOSED_EXPRESSION_H__
#define CENCLOSED_EXPRESSION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"
#include "CExpression.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a compound expression, composed of multiple symbols (and/or sub expressions) 
    ///        encapsulated by parenthesis.
    class CEnclosedExpression : public CExpression
    { 
    private:
        std::vector<CRawEntry> m_assignmentOperator;        
        std::vector<CLinkerScriptContentBase> m_composition;        
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CEnclosedExpression(CRawEntry openingParenthesis,
                                     CRawEntry closingParenthesis,
                                     std::vector<CLinkerScriptContentBase>&& composition,
                                     std::vector<CRawEntry>&& rawElements, 
                                     std::vector<CViolation>&& violations) 
            : CExpression(composition, rawElements, violations),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_composition(composition)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::EnclosedExpression;
        }    

        /// @brief Gets the Opening Parenthesis's CRawElement
        CRawElement& OpeningParenthesis() const
        {
            return this->m_openingParenthesis;
        }

        /// @brief Gets the Closing Parenthesis's CRawElement
        CRawElement& ClosingParenthesis() const
        {
            return this->m_closingParenthesis;
        }
    };
}

#endif
