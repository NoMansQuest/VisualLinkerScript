#include "CIncludeRescursionGuard.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CIncludeRescursionGuard)

using namespace VisualLinkerScript::DrcEngine::Rules;

std::vector<std::shared_ptr<CDrcViolation>> CIncludeRescursionGuard::PerformCheck(const CLinkerScriptFile& linkerScriptFile)
{
    std::vector<std::shared_ptr<CDrcViolation>> violations;



    return violations;
}

