#ifndef CMEMORYREGIONDEFINEDONLYONCERULE_H
#define CMEMORYREGIONDEFINEDONLYONCERULE_H

#include "DrcEngine/IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CMemoryRegionDefinedOnlyOnceRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CMemoryRegionDefinedOnlyOnceRule() = default;

        /// @copydoc
        std::string DrcRuleTitle() override
        {
            return "Memory Region Defined Only Once Rule";
        }

        /// @copydoc
        LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CMemoryRegionDefinedOnlyOnceRule() override = default;
    };
}

#endif // CMEMORYREGIONDEFINEDONLYONCERULE_H
