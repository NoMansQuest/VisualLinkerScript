#include "CLocationCounterCannotGoBackwardsRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CLocationCounterCannotGoBackwardsRule)

using namespace VisualLinkerScript::DrcEngine::Rules;

SharedPtrVector<CDrcViolation> CLocationCounterCannotGoBackwardsRule::PerformCheck(const CLinkerScriptFile& linkerScriptFile) {
    SharedPtrVector<CDrcViolation> violations;



    return violations;
}

