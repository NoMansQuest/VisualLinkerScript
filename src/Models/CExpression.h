#ifndef CEXPRESSION_H__
#define CEXPRESSION_H__

#include <vector>
#include "CParsedContentBase.h"
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models
{
    /// @brief Represents a compound R-Value expression, composed of multiple symbols (and/or sub expressions).
    class CExpression: public CParsedContentBase
    {   
    private:
        LinqVector<CParsedContentBase> m_composition;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;        

    public:
        /// @brief Parameterized constructor, accessible to inheritors only
        explicit CExpression(const CRawEntry& openingParenthesis,
                             const CRawEntry& closingParenthesis,
                             const LinqVector<CParsedContentBase>& composition,
                             const std::vector<CRawEntry>& rawElements,
                             const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_composition(composition),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis)
        {}        

        /// @brief Simplified constructor, no parenthesis
        explicit CExpression(LinqVector<CParsedContentBase> composition,
                             const std::vector<CRawEntry>& rawElements,
                             const LinqVector<CViolationBase>& violations)
    		: CParsedContentBase(rawElements, violations),
              m_composition(std::move( composition))
        {}

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Expression;
        }    

        /// @brief Reports back opening-parenthesis
        [[nodiscard]] const CRawEntry& OpeningParenthesis() const
        {
            return this->m_openingParenthesis;
        }

        /// @brief Reports back closing parenthesis
        [[nodiscard]] const CRawEntry& ClosingParenthesis() const
        {
            return this->m_closingParenthesis;
        }

        /// @brief Gets the "Composition" 
        [[nodiscard]] const LinqVector<CParsedContentBase>& Composition() const
        {
            return this->m_composition;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
