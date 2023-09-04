#include "CDrcManager.h"

using namespace VisualLinkerScript::DrcEngine;

/// @copydoc
bool CDrcManager::RegisterRule(std::shared_ptr<IDrcRuleBase> drcRule) {
    CDrcManager::Instance().m_drcRules.emplace_back(drcRule);
    return true;
}

/// @copydoc
SharedPtrVector<CViolationBase> CDrcManager::Violations(){
    return this->m_violations;
}
