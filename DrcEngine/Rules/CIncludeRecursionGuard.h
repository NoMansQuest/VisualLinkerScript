#ifndef CINCLUDE_RECURSION_GUARD_H__
#define CINCLUDE_RECURSION_GUARD_H__

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CIncludeRecursionGuard : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CIncludeRecursionGuard()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Include Recursions Guard Rule";
        }

        /// @copydoc
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const std::vector<std::shared_ptr<CLinkerScriptFile>>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CIncludeRecursionGuard()
        {}
    };
}

#endif // CINCLUDE_RECURSION_GUARD_H__