#ifndef CEVALUATION_RESULT_H__
#define CEVALUATION_RESULT_H__

#include <vector>
#include "EvaluationResultType.h"
#include "ArithmeticOperationType.h"

namespace VisualLinkerScript::Models::Evaluation
{    
    /// @brief Abstract base object for results produced by having @see {CEvaluatableBase} 
    ///        objects evaluataed.
    class CResultBase
    {
    public:
        /// @brief Type of operation.        
        virtual EvaluationResultType EvaluationType() = 0;    
    };
}

#endif
