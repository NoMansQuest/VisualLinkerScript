#include "DrcEngine/Rules/CEntryIsDefinedRule.h"
#include "DrcEngine/DrcCommons.h"
#include "DrcEngine/CDrcManager.h"
#include "Models/CLinkerScriptFile.h"
#include "Models/CFunctionCall.h"
#include "Helpers.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"

REGISTER_DRC_RULE(CEntryIsDefinedRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

LinqVector<CViolationBase> CEntryIsDefinedRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    LinqVector<CViolationBase> violations;

	auto foundEntryCalls = linkerScriptFile->ParsedContent()
		.OfType<CFunctionCall>()
		.Where([&](const std::shared_ptr<CFunctionCall>& call)
		{
			return StringEquals(linkerScriptFile->ResolveRawEntry(call->FunctionName()), "ENTRY", true);
		});

	if (foundEntryCalls.size() > 1)
	{
		for (const auto foundDirective : foundEntryCalls)
		{
			violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
				foundDirective,
				this->DrcRuleTitle(),
				"Multiple definitions of 'ENTRY' directive was detected.",
				EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce,
				ESeverityCode::Error))));
		}		
	}

    return violations;
}



