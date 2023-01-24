#ifndef CEXPRESSION_OPERATOR_H__
#define CEXPRESSION_OPERATOR_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a operator which is used in expression
    class CExpressionOperator : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_expressionOperator;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CExpressionOperator(CRawEntry expressionOperator,
                                     std::vector<CRawEntry>&& rawElements,
                                     std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_expressionOperator(expressionOperator)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SecondarySymbol;
        }

        /// @brief Gets the operator itself
        const CRawEntry& ExpressionOperator()
        {
            return this->m_expressionOperator;
        }
    };
}

#endif
