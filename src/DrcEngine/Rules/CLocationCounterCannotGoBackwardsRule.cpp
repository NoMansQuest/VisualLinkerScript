#include "DrcEngine/Rules/CLocationCounterCannotGoBackwardsRule.h"
#include "DrcEngine/DrcCommons.h"
#include "DrcEngine/CDrcManager.h"

REGISTER_DRC_RULE(CLocationCounterCannotGoBackwardsRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

LinqVector<CViolationBase> CLocationCounterCannotGoBackwardsRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    // TODO: TO BE IMPLEMENTED
    LinqVector<CViolationBase> violations;
    return violations;
}

