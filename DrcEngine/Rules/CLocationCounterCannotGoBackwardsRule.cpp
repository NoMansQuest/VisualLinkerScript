#include "CLocationCounterCannotGoBackwardsRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CLocationCounterCannotGoBackwardsRule)

using namespace VisualLinkerScript::DrcEngine::Rules;

std::vector<std::shared_ptr<CDrcViolation>> CLocationCounterCannotGoBackwardsRule::PerformCheck(const CLinkerScriptFile& linkerScriptFile) {
    std::vector<std::shared_ptr<CDrcViolation>> violations;



    return violations;
}

