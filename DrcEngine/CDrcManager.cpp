#include "CDrcManager.h"
#include "../Models/CViolationBase.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::DrcEngine;

SharedPtrVector<IDrcRuleBase> CDrcManager::m_drcRules;

/// @copydoc
bool CDrcManager::RegisterRule(const std::shared_ptr<IDrcRuleBase>& drcRule) {
    CDrcManager::m_drcRules.emplace_back(drcRule);
    return true;
}

/// @copydoc
SharedPtrVector<CViolationBase> CDrcManager::Violations(){
    return this->m_violations;
}
