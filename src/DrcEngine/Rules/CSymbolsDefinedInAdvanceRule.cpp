#include "DrcEngine/Rules/CSymbolsDefinedInAdvanceRule.h"
#include "Models/CSectionsRegion.h"
#include "Helpers.h"

#include "DrcEngine/DrcCommons.h"
#include "DrcEngine/CDrcManager.h"
#include "DrcEngine/IDrcRuleBase.h"

REGISTER_DRC_RULE(CSymbolsDefinedInAdvanceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

LinqVector<CViolationBase> CSymbolsDefinedInAdvanceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    LinqVector<CViolationBase> violations;

    /* TO BE IMPLEMENTED IN THE FUTURE */
    return violations;
}
