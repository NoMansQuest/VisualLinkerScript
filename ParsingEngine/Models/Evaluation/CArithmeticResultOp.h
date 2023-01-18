#ifndef EVALUATION_ARITHMETIC_OPERATION_H__
#define EVALUATION_ARITHMETIC_OPERATION_H__

#include "CResultBase.h"
#include "CEvaluatableBase.h"
#include "ArithmeticOperationType.h"

namespace VisualLinkerScript::ParsingEngine::Models::Evaluation
{
    /// @brief Represents an arithmetic operation inbetween results. If not present, the then
    ///        default operation is addition
    class CArithmeticResultOp : CResultBase
    {
    private:    
        ArithmeticOperationType m_type;        

    public:
        /// @brief Default constructor        
        CArithmeticResultOp(ArithmeticOperationType type) 
            : m_type(type)
        {}

    public:
        /// @brief Returns the type of the operation
        ArithmeticOperationType OperationType() 
        {
            return m_type;
        }       

        /// @copydoc CEvaluatedResultBase::Type()
        EvaluationResultType EvaluationType()
        {
            return EvaluationResultType::ArithmetciOperation;
        }
    };
}


#endif
