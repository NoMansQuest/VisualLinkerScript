#ifndef ARITHMETIC_OPERATION_TYPE__
#define ARITHMETIC_OPERATION_TYPE__

namespace VisualLinkerScript::Models::Evaluation
{
    /// @brief Evluation
    enum class ArithmeticOperationType
    {
        Addition,
        Subtraction,
        Division,
        Multiplication,
        LeftShift,
        RightShift,
        LogicalOr,
        LogicalAnd,
        TernaryIf,
        TernaryElse
    };
}

#endif
