#ifndef CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H
#define CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H

#include "DrcEngine/IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CLocationCounterCannotGoBackwardsRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CLocationCounterCannotGoBackwardsRule() = default;

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Entry Is Defined Rule";
        }

        /// @copydoc
        virtual LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CLocationCounterCannotGoBackwardsRule() override = default;
    };
}


#endif // CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H
