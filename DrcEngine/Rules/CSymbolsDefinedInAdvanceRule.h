#ifndef CSYMBOLSAREDEFINEDINADVANCERULE_H
#define CSYMBOLSAREDEFINEDINADVANCERULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CSymbolsDefinedInAdvanceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CSymbolsDefinedInAdvanceRule()
        {}

        /// @copydoc
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CSymbolsDefinedInAdvanceRule()
        {}
    };
}


#endif // CSYMBOLSAREDEFINEDINADVANCERULE_H
