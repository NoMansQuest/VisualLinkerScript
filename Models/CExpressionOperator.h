#ifndef CEXPRESSION_OPERATOR_H__
#define CEXPRESSION_OPERATOR_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a evaluative and arithmetic operators which is used in expressions.
    class CExpressionOperator : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_expressionOperator;

    public:
        /// @brief Default constructor.
        explicit CExpressionOperator(CRawEntry expressionOperator,
                                     std::vector<CRawEntry>&& rawElements,
                                     std::vector<CParserViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_expressionOperator(expressionOperator)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::ArithmeticOrLogicalOperator;
        }

        /// @brief Gets the operator itself
        const CRawEntry& ExpressionOperator()
        {
            return this->m_expressionOperator;
        }
    };
}

#endif
