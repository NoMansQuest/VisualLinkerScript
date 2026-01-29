#ifndef CDIRECTIVESDECLAREDONLYONCERULE_H
#define CDIRECTIVESDECLAREDONLYONCERULE_H

#include "DrcEngine/IDrcRuleBase.h"

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
        LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CDirectivesDeclaredOnlyOnceRule() override = default;
    };
}
#endif // CDIRECTIVESDECLAREDONLYONCERULE_H
