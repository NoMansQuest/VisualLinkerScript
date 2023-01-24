#ifndef CEXPRESSION_NUMBER_H__
#define CEXPRESSION_NUMBER_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a Number which is used in expression
    class CExpressionNumber : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_expressionNumber;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CExpressionNumber(CRawEntry expressionNumber,
                                   std::vector<CRawEntry>&& rawElements,
                                   std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_expressionNumber(expressionNumber)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SecondarySymbol;
        }

        /// @brief Gets the number Raw-Entry
        const CRawEntry& ExpressionNumber()
        {
            return this->m_expressionNumber;
        }
    };
}

#endif
