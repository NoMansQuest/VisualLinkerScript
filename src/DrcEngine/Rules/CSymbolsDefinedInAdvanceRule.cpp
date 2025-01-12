#include "CSymbolsDefinedInAdvanceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CSymbolsDefinedInAdvanceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

LinqVector<CViolationBase> CSymbolsDefinedInAdvanceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    LinqVector<CViolationBase> violations;

    /* TO BE IMPLEMENTED IN THE FUTURE */
    return violations;
}
