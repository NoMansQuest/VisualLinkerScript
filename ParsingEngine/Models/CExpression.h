#ifndef CEXPRESSION_H__
#define CEXPRESSION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a compound R-Value expression, composed of multiple symbols (and/or sub expressions).
    class CExpression: public CLinkerScriptContentBase
    {   
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_composition;
        CRawEntry m_openningParenthesis;
        CRawEntry m_closingParenthesis;        

    public:
        /// @brief Parameterized constructor, accessible to inheritors only
        explicit CExpression(CRawEntry openningParenthesis,
                             CRawEntry closingParenthesis,
                             std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& composition,
                             std::vector<CRawEntry>&& rawElements, 
                             std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_composition(composition),
              m_openningParenthesis(openningParenthesis),
              m_closingParenthesis(closingParenthesis)
        {}        

        /// @brief Simplified constructor, no parenthesis
        explicit CExpression(std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& composition,
                             std::vector<CRawEntry>&& rawElements,
                             std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_composition(composition)
        {}

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Expression;
        }    

        /// @brief Reports back openning-parenthesis
        const CRawEntry& OpenningParenthesis()
        {
            return this->m_openningParenthesis;
        }

        /// @brief Reports back closing parenthesis
        const CRawEntry& ClosingParenthesis()
        {
            return this->m_closingParenthesis;
        }

        /// @brief Gets the "Composition" 
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Composition()
        {
            return this->m_composition;
        }
    };
}

#endif
