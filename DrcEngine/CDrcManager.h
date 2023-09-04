#ifndef CDRCENGINEMANAGER_H__
#define CDRCENGINEMANAGER_H__

#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include "../Helpers.h"
#include "IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine
{
    using namespace VisualLinkerScript;

    /// @brief In charge of Design-Rule-Check operations
    class CDrcManager
    {
        typedef std::vector<std::shared_ptr<IDrcRuleBase>> DrcRulesType;

    private:
        DrcRulesType m_drcRules;
        SharedPtrVector<CViolationBase> m_violations;

    public:
        /// @brief Implementation of the singleton
        static CDrcManager& Instance() {
            static CDrcManager drcManager;
            return drcManager;
        }

        /// @brief Registers a class as DRC-Rule
        bool RegisterRule(std::shared_ptr<IDrcRuleBase> drcRule);

        /// @brief Reports back a list of currently identified violations
        SharedPtrVector<CViolationBase> Violations();
    };
}


#endif // CDRCENGINEMANAGER_H__
