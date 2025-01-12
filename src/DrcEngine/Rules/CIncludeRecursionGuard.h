#ifndef CINCLUDE_RECURSION_GUARD_H__
#define CINCLUDE_RECURSION_GUARD_H__

#include "../IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CIncludeRecursionGuard : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CIncludeRecursionGuard() = default;

        /// @copydoc
        std::string DrcRuleTitle() override
        {
            return "Include Recursions Guard Rule";
        }

        /// @copydoc
        LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CIncludeRecursionGuard() override = default;
    };
}

#endif // CINCLUDE_RECURSION_GUARD_H__
