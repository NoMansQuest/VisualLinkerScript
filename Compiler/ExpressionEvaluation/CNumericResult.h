#ifndef CEVALUATED_AS_NUMERIC_RESULT_H__
#define CEVALUATED_AS_NUMERIC_RESULT_H__

#include "CResultBase.h"

namespace VisualLinkerScript::ParsingEngine::Models::Evaluation
{
    /// @brief Produced by having @see {CEvaluatableBase} objects evaluataed.
    class CNumericResult : CResultBase
    {
    private:
        uint64_t m_numericResult;
        bool m_isNegative;
        
    public:
        /// @brief Default constructor
        CNumericResult(uint64_t numericResult, bool isNegative) 
            : m_numericResult(numericResult),
              m_isNegative(isNegative)
        {}

    public:
        /// @brief Returns the numeric value of this result.
        uint64_t NumericResult()
        {
            return m_numericResult;
        }

        /// @brief Reports back whether the number is negative or not 
        bool IsNegative()
        {
            return m_isNegative;
        }

        /// @copydoc CEvaluatedResultBase::Type()
        virtual EvaluationResultType EvaluationType() override
        {
            return EvaluationResultType::NumericResult;
        }
    };
}

#endif
