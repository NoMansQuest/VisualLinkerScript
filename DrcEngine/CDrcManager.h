#ifndef CDRCENGINEMANAGER_H__
#define CDRCENGINEMANAGER_H__

#include <memory>
#include "../Helpers.h"

namespace VisualLinkerScript
{
    namespace Models
    {
        class CViolationBase;
    }

    namespace DrcEngine
    {
	    class IDrcRuleBase;

	    /// @brief In charge of Design-Rule-Check operations
        class CDrcManager
        {
            typedef SharedPtrVector<IDrcRuleBase> DrcRulesType;
            static DrcRulesType m_drcRules;
            SharedPtrVector<Models::CViolationBase> m_violations;

        public:
            /// @brief Default constructor;
            CDrcManager() = default;

            /// @brief Registers a class as DRC-Rule
            static bool RegisterRule(const std::shared_ptr<IDrcRuleBase>& drcRule);

            /// @brief Reports back a list of currently identified violations
            SharedPtrVector<Models::CViolationBase> Violations();
        };
    }
}

#endif // CDRCENGINEMANAGER_H__
