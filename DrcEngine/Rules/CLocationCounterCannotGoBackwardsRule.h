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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CLocationCounterCannotGoBackwardsRule()
        {}
    };
}


#endif // CLOCATIONCOUNTERCANNOTGOBACKWARDSRULE_H
