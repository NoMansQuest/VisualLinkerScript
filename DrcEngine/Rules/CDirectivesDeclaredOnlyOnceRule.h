#ifndef CDIRECTIVESDECLAREDONLYONCERULE_H
#define CDIRECTIVESDECLAREDONLYONCERULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CDirectivesDeclaredOnlyOnceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CDirectivesDeclaredOnlyOnceRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Directives Defined Only Once Rule";
        }

        /// @copydoc
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const std::vector<std::shared_ptr<CLinkerScriptFile>>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CDirectivesDeclaredOnlyOnceRule()
        {}
    };
}
#endif // CDIRECTIVESDECLAREDONLYONCERULE_H
