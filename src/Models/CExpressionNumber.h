#ifndef CEXPRESSION_NUMBER_H__
#define CEXPRESSION_NUMBER_H__

#include <vector>
#include "Models/CParsedContentBase.h"
#include "Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Number which is used in an expression
    class CExpressionNumber : public CParsedContentBase
    {
    private:
        CRawEntry m_expressionNumber;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CExpressionNumber(const CRawEntry& expressionNumber,
                                   const std::vector<CRawEntry>& rawElements,
                                   const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_expressionNumber(expressionNumber)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::Number;
        }


        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override
        {
            return this->Violations();
        }

        /// @brief Gets the number Raw-Entry
        const CRawEntry& ExpressionNumber()
        {
            return this->m_expressionNumber;
        }
    };
}

#endif
