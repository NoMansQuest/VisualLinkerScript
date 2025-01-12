#ifndef IDRCRULEBASE_H
#define IDRCRULEBASE_H

#include <vector>
#include <cstdint>
#include "../Helpers.h"
#include "Models/CViolationBase.h"
#include "Models/CParsedContentBase.h"

namespace VisualLinkerScript::DrcEngine
{
    class CDrcViolation;
	using namespace VisualLinkerScript::Models;

    /// @brief Base class for all DRC Rule checkers
    class IDrcRuleBase
    {
        bool m_isEnabled;

    public:
       
        /// @brief Perform the check and report back found violations
        virtual LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) = 0;

        /// @brief Updated 'Enabled' state of the rule.
        virtual void SetEnabled(const bool enabled){
            this->m_isEnabled = enabled;
        }

        /// @brief If violation can be automatically corrected, this object will represent it.
        virtual bool IsEnabled(){
            return this->m_isEnabled;
        }

        /// @brief Returns the title of the DRC rule, which will be shown in the DRC-Violations.
        virtual std::string DrcRuleTitle() = 0;

        /// @brief Default destructor;
        virtual ~IDrcRuleBase() = default;
    };
}

#endif // IDRCRULEBASE_H
