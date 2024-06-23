#ifndef CDIRECTIVESDECLAREDONLYONCERULE_H
#define CDIRECTIVESDECLAREDONLYONCERULE_H

#include "../IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CDirectivesDeclaredOnlyOnceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CDirectivesDeclaredOnlyOnceRule() = default;

        /// @copydoc
        std::string DrcRuleTitle() override
        {
            return "Directives Defined Only Once Rule";
        }

        /// @copydoc
        SharedPtrVector<CViolationBase> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CDirectivesDeclaredOnlyOnceRule() override = default;
    };
}
#endif // CDIRECTIVESDECLAREDONLYONCERULE_H
