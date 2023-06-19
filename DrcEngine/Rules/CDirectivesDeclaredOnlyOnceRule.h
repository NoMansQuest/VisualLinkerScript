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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CDirectivesDeclaredOnlyOnceRule()
        {}
    };
}
#endif // CDIRECTIVESDECLAREDONLYONCERULE_H
