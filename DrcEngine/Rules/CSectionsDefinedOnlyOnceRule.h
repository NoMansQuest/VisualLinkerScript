#ifndef CSECTIONSDEFINEDONLYONCERULE_H
#define CSECTIONSDEFINEDONLYONCERULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CSectionsDefinedOnlyOnceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CSectionsDefinedOnlyOnceRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Sections Region Defined Only Once Rule";
        }

        /// @copydoc
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CSectionsDefinedOnlyOnceRule()
        {}
    };
}


#endif // CSECTIONSDEFINEDONLYONCERULE_H
