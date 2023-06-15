#ifndef CDRCENGINEMANAGER_H__
#define CDRCENGINEMANAGER_H__

#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include "CDrcViolation.h"
#include "IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine
{
    using namespace VisualLinkerScript::DrcEngine;

    /// @brief In charge of Design-Rule-Check operations
    class CDrcManager
    {
        typedef std::vector<std::shared_ptr<IDrcRuleBase>> DrcRulesType;

    private:
        DrcRulesType m_drcRules;
        std::vector<std::shared_ptr<CDrcViolation>> m_violations;

    public:
        /// @brief Implementation of the singleton
        static CDrcManager& Instance() {
            static CDrcManager drcManager;
            return drcManager;
        }

        /// @brief Registers a class as DRC-Rule
        bool RegisterInterface(std::shared_ptr<IDrcRuleBase> drcRule);

        /// @brief Reports back a list of currently identified violations
        std::vector<std::shared_ptr<CDrcViolation>> Violations();
    };
}


#endif // CDRCENGINEMANAGER_H__
