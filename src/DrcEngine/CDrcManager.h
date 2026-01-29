#ifndef CDRCENGINEMANAGER_H__
#define CDRCENGINEMANAGER_H__

#include <memory>
#include "Helpers.h"

namespace VisualLinkerScript
{
    namespace Models
    {
	    class CLinkerScriptFile;
	    class CViolationBase;
    }

    namespace DrcEngine
    {
	    class IDrcRuleBase;

	    /// @brief In charge of Design-Rule-Check operations
        class CDrcManager
        {
            typedef LinqVector<IDrcRuleBase> DrcRulesType;
            static DrcRulesType m_drcRules;

        public:
            /// @brief Default constructor;
            CDrcManager() = default;

            /// @brief Registers a class as DRC-Rule
            static bool RegisterRule(const std::shared_ptr<IDrcRuleBase>& drcRule);

            /// @brief Reports back a list of currently identified violations
            void PerformAnalysis(std::shared_ptr<Models::CLinkerScriptFile> linkerScriptFile);
        };
    }
}

#endif // CDRCENGINEMANAGER_H__
