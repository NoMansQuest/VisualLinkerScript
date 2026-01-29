#ifndef CEXPRESSION_OPERATOR_H__
#define CEXPRESSION_OPERATOR_H__

#include <vector>
#include "Models/CParsedContentBase.h"
#include "Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents evaluative and arithmetic operators which is used in expressions.
    class CExpressionOperator : public CParsedContentBase
    {
    private:
        CRawEntry m_expressionOperator;

    public:
        /// @brief Default constructor.
        explicit CExpressionOperator(const CRawEntry& expressionOperator,
                                     const std::vector<CRawEntry>& rawElements,
                                     const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_expressionOperator(expressionOperator)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::ArithmeticOrLogicalOperator;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override
        {
            return this->Violations();
        }

        /// @brief Gets the operator itself
        const CRawEntry& ExpressionOperator()
        {
            return this->m_expressionOperator;
        }
    };
}

#endif
