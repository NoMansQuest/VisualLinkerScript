#ifndef CEXPRESSION_H__
#define CEXPRESSION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models
{
    /// @brief Represents a compound R-Value expression, composed of multiple symbols (and/or sub expressions).
    class CExpression: public CLinkerScriptContentBase
    {   
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_composition;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;        

    public:
        /// @brief Parameterized constructor, accessible to inheritors only
        explicit CExpression(CRawEntry openingParenthesis,
                             CRawEntry closingParenthesis,
                             std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& composition,
                             std::vector<CRawEntry>&& rawElements, 
                             SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_composition(composition),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis)
        {}        

        /// @brief Simplified constructor, no parenthesis
        explicit CExpression(std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& composition,
                             std::vector<CRawEntry>&& rawElements,
                             SharedPtrVector<CViolationBase>&& violations)
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
        const CRawEntry& OpeningParenthesis() const
        {
            return this->m_openingParenthesis;
        }

        /// @brief Reports back closing parenthesis
        const CRawEntry& ClosingParenthesis() const
        {
            return this->m_closingParenthesis;
        }

        /// @brief Gets the "Composition" 
        const SharedPtrVector<CLinkerScriptContentBase>& Composition() const
        {
            return this->m_composition;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
