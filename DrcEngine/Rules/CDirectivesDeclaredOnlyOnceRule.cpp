#include "CDirectivesDeclaredOnlyOnceRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CFunctionCall.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "DrcEngine/EDrcViolationSeverity.h"

REGISTER_DRC_RULE(CDirectivesDeclaredOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

SharedPtrVector<CViolationBase> CDirectivesDeclaredOnlyOnceRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CViolationBase> violations;

    std::vector<std::string> directives {
        "ENTRY",
        "OUTPUT",
        "OUTPUT_ARCH",
        "OUTPUT_FORMAT",
        "TARGET",
        "STARTUP",
        "SEARCH_DIR",
    };

    for (auto directive : directives) 
    {
	    if (auto foundDirectives = QueryObject<CFunctionCall>(
		    linkerScriptFiles,
		    [&](const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile, const std::shared_ptr<CFunctionCall>& ResolveEntryText) {
			    return StringEquals(linkerScriptFile->ResolveEntryText(ResolveEntryText->FunctionName()), directive, true);
		    }); 
            foundDirectives.size() > 1) 
        {
             violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
	             std::vector<std::shared_ptr<CLinkerScriptContentBase>>(),
	             this->DrcRuleTitle(),
	             "Directive is defined more than once.",
	             EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce,
	             EDrcViolationSeverity::Error))));
        }
    }

    return violations;
}
