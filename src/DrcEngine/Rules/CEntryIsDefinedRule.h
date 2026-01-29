#ifndef CENTRYISDEFINEDRULE_H
#define CENTRYISDEFINEDRULE_H

#include "DrcEngine/IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CEntryIsDefinedRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CEntryIsDefinedRule() = default;

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Entry Is Defined Rule";
        }

        /// @copydoc
        virtual LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CEntryIsDefinedRule() override = default;
    };
}

#endif // CENTRYISDEFINEDRULE_H
