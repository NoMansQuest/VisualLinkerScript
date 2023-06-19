#include "CDirectivesDeclaredOnlyOnceRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CDirectivesDeclaredOnlyOnceRule)

using namespace VisualLinkerScript::DrcEngine::Rules;

/// @brief Perform the check and report back found violations
std::vector<std::shared_ptr<CDrcViolation>> CDirectivesDeclaredOnlyOnceRule::PerformCheck(const CLinkerScriptFile& linkerScriptFile)
{
    std::vector<std::shared_ptr<CDrcViolation>> violations;



    return violations;
}


