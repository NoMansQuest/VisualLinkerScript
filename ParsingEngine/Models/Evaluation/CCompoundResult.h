#ifndef EVALUATION_ARITHMETIC_OPERATION_H__
#define EVALUATION_ARITHMETIC_OPERATION_H__

#include "CResultBase.h"
#include "EvaluationResultType.h"

namespace VisualLinkerScript::ParsingEngine::Models::Evaluation
{
    /// @brief Produced by having @see {CEvaluatableBase} objects evaluataed.
    class CCompoundResult : CResultBase
    {
    private:
        std::vector<CEvaluationResultBase> m_composition;       
        
    public:
        /// @brief Default constructor
        CCompoundResult(std::vector<CEvaluationResultBase> composition)
            : m_composition(composition)
        {}

        /// @brief Reports back the composition of this evaluation
        const std::vector<CEvaluationResultBase>& Composition()
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