#include "CSymbolsDefinedInAdvanceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CSymbolsDefinedInAdvanceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;

LinqVector<CViolationBase> CSymbolsDefinedInAdvanceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    LinqVector<CViolationBase> violations;

    /* TO BE IMPLEMENTED IN THE FUTURE */
    return violations;
}
