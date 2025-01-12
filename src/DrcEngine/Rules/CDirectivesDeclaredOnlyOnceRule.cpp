#include "CDirectivesDeclaredOnlyOnceRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CFunctionCall.h"
#include "../../Helpers.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"

REGISTER_DRC_RULE(CDirectivesDeclaredOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

LinqVector<CViolationBase> CDirectivesDeclaredOnlyOnceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    LinqVector<CViolationBase> violations;    
 
    std::vector<std::string> directives {
        "ENTRY",
        "OUTPUT",
        "OUTPUT_ARCH",
        "OUTPUT_FORMAT",
        "TARGET",
        "STARTUP",
    };

    for (const auto& directive : directives) 
    {
        auto func = [&](const std::shared_ptr<CFunctionCall>& functionCall) -> bool
    	{
            return StringEquals(linkerScriptFile->ResolveRawEntry(functionCall->FunctionName()), directive, true);
        };

	    if (auto foundDirectives = linkerScriptFile->ParsedContent()
            .OfType<CFunctionCall>()
            .Select(func);
            foundDirectives.size() > 1) 
        {
             violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                 LinqVector<CParsedContentBase>(),
	             this->DrcRuleTitle(),
	             "Directive is defined more than once.",
	             EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce,
	             ESeverityCode::Error))));
        }
    }
  
    return violations;
}
