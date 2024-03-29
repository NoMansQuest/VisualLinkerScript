#ifndef CENTRYISDEFINEDRULE_H
#define CENTRYISDEFINEDRULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CEntryIsDefinedRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CEntryIsDefinedRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Entry Is Defined Rule";
        }

        /// @copydoc
        virtual SharedPtrVector<CViolationBase> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CEntryIsDefinedRule()
        {}
    };
}

#endif // CENTRYISDEFINEDRULE_H
