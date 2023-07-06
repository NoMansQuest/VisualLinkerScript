#ifndef IDRCRULEBASE_H
#define IDRCRULEBASE_H

#include <vector>
#include <cstdint>
#include <DrcEngine/CDrcViolation.h>
#include "../Helpers.h"
#include "../Models/CLinkerScriptFile.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Intervention;

namespace VisualLinkerScript::DrcEngine
{
    /// @brief Base class for all DRC Rule checkers
    class IDrcRuleBase
    {
    private:
        bool m_isEnabled;

    public:
        /// @brief Perform the check and report back found violations
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) = 0;

        /// @brief Updated 'Enabled' state of the rule.
        virtual void SetEnabled(bool enabled){
            this->m_isEnabled = enabled;
        }

        /// @brief If violation can be automatically corrected, this object will represent it.
        virtual bool IsEnabled(){
            return this->m_isEnabled;
        };

        /// @brief Returns the title of the DRC rule, which will be shown in the DRC-Violations.
        virtual std::string DrcRuleTitle() = 0;
    };
}

#endif // IDRCRULEBASE_H
