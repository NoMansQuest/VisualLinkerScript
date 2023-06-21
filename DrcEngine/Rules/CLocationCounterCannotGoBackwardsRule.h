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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const std::vector<std::shared_ptr<CLinkerScriptFile>>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CLocationCounterCannotGoBackwardsRule()
        {}
    };
}


#endif // CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H
