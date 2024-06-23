#ifndef CSYMBOLSAREDEFINEDINADVANCERULE_H
#define CSYMBOLSAREDEFINEDINADVANCERULE_H

#include "../IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CSymbolsDefinedInAdvanceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CSymbolsDefinedInAdvanceRule() = default;

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Symbols Defined In Advance Rule";
        }

        /// @copydoc
        virtual SharedPtrVector<CViolationBase> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CSymbolsDefinedInAdvanceRule() override = default;
    };
}


#endif // CSYMBOLSAREDEFINEDINADVANCERULE_H
