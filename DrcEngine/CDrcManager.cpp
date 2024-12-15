#include "CDrcManager.h"

#include "IDrcRuleBase.h"
#include "../Models/CViolationBase.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::DrcEngine;

SharedPtrVector<IDrcRuleBase> CDrcManager::m_drcRules;

/// @copydoc
bool CDrcManager::RegisterRule(const std::shared_ptr<IDrcRuleBase>& drcRule)
{
    CDrcManager::m_drcRules.emplace_back(drcRule);
    return true;
}

/// @copydoc
void CDrcManager::PerformAnalysis(std::shared_ptr<CLinkerScriptFile> linkerScriptFile)
{
	SharedPtrVector<CViolationBase> discoveredViolations;
	for (const auto& rule : this->m_drcRules)
	{
		if (!rule->IsEnabled())
		{
			continue;
		}

		auto violations = rule->PerformCheck(linkerScriptFile);
		if (violations.empty())
		{
			continue;
		}

		discoveredViolations.insert(discoveredViolations.end(), violations.begin(), violations.end());
	}	
}
