#ifndef CEVALUATED_AS_NUMERIC_RESULT_H__
#define CEVALUATED_AS_NUMERIC_RESULT_H__

#include <string>
#include "CResultBase.h"

namespace VisualLinkerScript::Models::Evaluation
{
    /// @brief Produced by having @see {CEvaluatableBase} objects evaluataed.
    class CSymbolResult : CResultBase
    {
    private:
        std::string m_symbolName;
        bool m_isNegative;
        
    public:
        /// @brief Default constructor
        CSymbolResult(std::string symbolName, bool isNegative)
            : m_symbolName(symbolName),
              m_isNegative(isNegative)

        {}

    public:
        /// @brief Returns the name of symbol
        const std::string& SymbolName()
        {
            return m_symbolName;
        }

        /// @brief Reports back whether the symbol is negated
        bool IsNegative()
        {
            return m_isNegative;
        }

        /// @copydoc CEvaluatedResultBase::Type()
        virtual EvaluationResultType EvaluationType() override
        {
            return EvaluationResultType::SymbolResult;
        }
    };
}

#endif
