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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CSectionsDefinedOnlyOnceRule()
        {}
    };
}


#endif // CSECTIONSDEFINEDONLYONCERULE_H
