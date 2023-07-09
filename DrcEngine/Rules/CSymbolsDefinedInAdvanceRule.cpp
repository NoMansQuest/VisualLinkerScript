#include "CSymbolsDefinedInAdvanceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CSymbolsDefinedInAdvanceRule)

using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript;

using namespace VisualLinkerScript::DrcEngine::Rules;


SharedPtrVector<CDrcViolation> CSymbolsDefinedInAdvanceRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CDrcViolation> violations;

    /* TO BE IMPLEMENTED IN THE FUTURE */
    return violations;
}
