#ifndef CSECTIONSDEFINEDONLYONCERULE_H
#define CSECTIONSDEFINEDONLYONCERULE_H

#include "DrcEngine/IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CSectionsDefinedOnlyOnceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CSectionsDefinedOnlyOnceRule() = default;

        /// @copydoc
        std::string DrcRuleTitle() override
        {
            return "Sections Region Defined Only Once Rule";
        }

        /// @copydoc
        LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CSectionsDefinedOnlyOnceRule() override = default;
    };
}


#endif // CSECTIONSDEFINEDONLYONCERULE_H
