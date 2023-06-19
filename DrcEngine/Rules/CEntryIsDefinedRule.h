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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CEntryIsDefinedRule()
        {}
    };
}

#endif // CENTRYISDEFINEDRULE_H
