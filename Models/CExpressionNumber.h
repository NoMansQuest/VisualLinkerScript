#ifndef CEXPRESSION_NUMBER_H__
#define CEXPRESSION_NUMBER_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Number which is used in an expression
    class CExpressionNumber : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_expressionNumber;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CExpressionNumber(const CRawEntry& expressionNumber,
                                   const std::vector<CRawEntry>& rawElements,
                                   const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_expressionNumber(expressionNumber)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::Number;
        }

        /// @brief Gets the number Raw-Entry
        const CRawEntry& ExpressionNumber()
        {
            return this->m_expressionNumber;
        }
    };
}

#endif
