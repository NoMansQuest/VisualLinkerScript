#ifndef CMEMORYREGIONDEFINEDONLYONCERULE_H
#define CMEMORYREGIONDEFINEDONLYONCERULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CMemoryRegionDefinedOnlyOnceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CMemoryRegionDefinedOnlyOnceRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Memory Region Defined Only Once Rule";
        }

        /// @copydoc
        virtual SharedPtrVector<CDrcViolation> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CMemoryRegionDefinedOnlyOnceRule()
        {}
    };
}

#endif // CMEMORYREGIONDEFINEDONLYONCERULE_H
