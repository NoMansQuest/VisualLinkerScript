#ifndef CDRCENGINEMANAGER_H__
#define CDRCENGINEMANAGER_H__

#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include "Rules/IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    /// @brief In charge of Design-Rule-Check operations
    class CDrcManager
    {
        typedef std::vector<std::shared_ptr<IDrcRuleBase>> DrcRulesType;

        public:
            /// @brief Implementation of the singleton
            CDrcManager& instance() {
                static CDrcManager drcManager;
                return drcManager;
            }

            bool RegisterInterface(std::shared_ptr<IDrcRuleBase> drcRule)
            {
                CDrcManager::instance().m_drcRules.emplace_back(drcRule);
                return true;
            }

        private:
            DrcRulesType m_drcRules;
    };
}


#endif // CDRCENGINEMANAGER_H__
