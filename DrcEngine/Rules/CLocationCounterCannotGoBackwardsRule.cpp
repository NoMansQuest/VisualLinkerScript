#include "CLocationCounterCannotGoBackwardsRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../../QueryEngine/QueryCenter.h"

REGISTER_DRC_RULE(CLocationCounterCannotGoBackwardsRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;

SharedPtrVector<CViolationBase> CLocationCounterCannotGoBackwardsRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    // TODO: TO BE IMPLEMENTED
    SharedPtrVector<CViolationBase> violations;
    return violations;
}

