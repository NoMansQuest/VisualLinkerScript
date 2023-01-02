#ifndef EVALUATION_RESULT_TYPE_H__
#define EVALUATION_RESULT_TYPE_H__

namespace VisualLinkerScript::ParsingEngine::Models::Evaluation
{
    /// @brief Evluation
    enum class EvaluationResultType
    {
        /// @brief Essentially we have a concrete number as result
        NumericResult,

        /// @brief The result of the evaluation is a symbol 
        SymbolResult,

        /// @brief Some variables were not possible to have evaluated.
        CompoundResult,

        /// @brief Arithmetic operation, sandwiched between two other results
        ArithmetciOperation
    };
}


#endif