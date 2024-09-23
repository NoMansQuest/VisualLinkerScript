#include "CEntryIsDefinedRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CFunctionCall.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"

REGISTER_DRC_RULE(CEntryIsDefinedRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

SharedPtrVector<CViolationBase> CEntryIsDefinedRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    SharedPtrVector<CViolationBase> violations;

	/*
    auto foundDirectives = QueryObject<CFunctionCall>(
		linkerScriptFile,
	    [&](const std::shared_ptr<CLinkerScriptFile>& localLinkerScriptFile, const std::shared_ptr<CFunctionCall>& resolveEntryText) {
		    return StringEquals(localLinkerScriptFile->ResolveEntryText(resolveEntryText->FunctionName()), "ENTRY", true);
	    });

	if (foundDirectives.size() > 1)
	{
		for (const auto foundDirective : foundDirectives)
		{
			violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
				foundDirective,
				this->DrcRuleTitle(),
				"Directive is defined more than once.",
				EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce,
				EDrcViolationSeverity::Error))));
		}		
	}
	*/

    return violations;
}



