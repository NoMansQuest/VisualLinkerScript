#include "CEntryIsDefinedRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../../Models/CLinkerScriptFile.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../IDrcRuleBase.h"
#include "../../Models/CFunctionCall.h"

REGISTER_DRC_RULE(CEntryIsDefinedRule)

using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

/// @brief Perform the check and report back found violations
std::vector<std::shared_ptr<CDrcViolation>> CEntryIsDefinedRule::PerformCheck(
        const CLinkerScriptFile& linkerScriptFile,
        const )
{
    std::vector<std::shared_ptr<CDrcViolation>> violations;

    auto foundDirectives = QueryObject<CFunctionCall>( {linkerScriptFile}, [&](CFunctionCall& ResolveEntryText) {
        linkerScriptFile.ResolveEntryText(ResolveEntryText.ProcedureName()).
    });

    auto listOfEntryDirectives = linkerScriptFile.Content()

    return violations;
}



