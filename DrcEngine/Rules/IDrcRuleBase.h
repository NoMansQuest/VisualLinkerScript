#ifndef IDRCRULEBASE_H
#define IDRCRULEBASE_H

#include <vector>
#include <cstdint>
#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/Intervention/CIntervention.h"

using namespace VisualLinkerScript::Models;

namespace VisualLinkerScript::DrcEngine::Rules
{
    /// @brief Base class for all DRC Rule checkers
    class IDrcRuleBase
    {
    public:
        virtual size_t RuleHashCode() = 0;

        virtual bool CanCorrectViolation() = 0;

        virtual void PerformCheck(const CLinkerScriptFile& linkerScriptFile) = 0;
    };
}

#endif // IDRCRULEBASE_H
