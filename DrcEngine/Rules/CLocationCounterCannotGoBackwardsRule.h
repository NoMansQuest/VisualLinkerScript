#ifndef CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H
#define CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CLocationCounterCannotGoBackwardsRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CLocationCounterCannotGoBackwardsRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Entry Is Defined Rule";
        }

        /// @copydoc
        virtual SharedPtrVector<CViolationBase> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CLocationCounterCannotGoBackwardsRule()
        {}
    };
}


#endif // CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H
