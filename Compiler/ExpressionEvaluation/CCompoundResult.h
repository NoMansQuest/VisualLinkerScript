#ifndef EVALUATION_ARITHMETIC_OPERATION_H__
#define EVALUATION_ARITHMETIC_OPERATION_H__

#include "CResultBase.h"
#include "EvaluationResultType.h"
#include "CEvaluatableBase.h"

namespace VisualLinkerScript::Models::Evaluation
{
    /// @brief Produced by having @see {CEvaluatableBase} objects evaluataed.
    class CCompoundResult : CResultBase
    {
    private:
        std::vector<CEvaluatableBase> m_composition;
        
    public:
        /// @brief Default constructor
        CCompoundResult(std::vector<CEvaluatableBase> composition)
            : m_composition(composition)
        {}

        /// @brief Reports back the composition of this evaluation
        const std::vector<CEvaluatableBase>& Composition()
        {
            return this->m_composition;
        }

        /// @copydoc CEvaluatedResultBase::Type()
        virtual EvaluationResultType EvaluationType() override
        {
            return EvaluationResultType::CompoundResult;
        }
    };
}


#endif
