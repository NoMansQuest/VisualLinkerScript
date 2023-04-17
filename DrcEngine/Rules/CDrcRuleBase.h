#ifndef CDRCRULEBASE_H
#define CDRCRULEBASE_H

#include "../../ParsingEngine/Models/CLinkerScriptFile.h"

using namespace VisualLinkerScript::ParsingEngine::Models;

namespace VisualLinkerScript::DrcEngine::Rules
{
    /// @brief Base class for all DRC Rule checkers
    class CDrcRuleBase
    {
    public:
        void PerformCheck(const CLinkerScriptFile& linkerScriptFile);
    };
}

#endif // CDRCRULEBASE_H
