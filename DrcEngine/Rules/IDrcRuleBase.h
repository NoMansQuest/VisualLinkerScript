#ifndef IDRCRULEBASE_H
#define IDRCRULEBASE_H

#include <vector>
#include <cstdint>
#include <DrcEngine/CDrcViolation.h>
#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/Intervention/CIntervention.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Intervention;

namespace VisualLinkerScript::DrcEngine::Rules
{
    /// @brief Base class for all DRC Rule checkers
    class IDrcRuleBase
    {
    public:
        /// @brief Hash code could be used to sort the error type
        virtual size_t RuleHashCode() = 0;

        /// @brief Is corrective action supported for this error?
        virtual bool CanCorrectViolation() = 0;

        /// @brief Perform the check and report back found violations
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) = 0;

        /// @brief If violation can be automatically corrected, this object will represent it.
        virtual CIntervention CorrectiveAction() = 0;
    };
}

#endif // IDRCRULEBASE_H
