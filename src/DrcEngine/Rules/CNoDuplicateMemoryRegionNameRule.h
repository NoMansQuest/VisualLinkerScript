#ifndef CNODUPLICATEMEMORYREGIONNAMERULE_H
#define CNODUPLICATEMEMORYREGIONNAMERULE_H

#include "DrcEngine/IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CNoDuplicateMemoryRegionNameRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CNoDuplicateMemoryRegionNameRule() = default;

        /// @copydoc
        std::string DrcRuleTitle() override
        {
            return "No Duplicate Memory Region Name Rule";
        }

        /// @copydoc
        LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CNoDuplicateMemoryRegionNameRule() override = default;
    };
}


#endif // CNODUPLICATEMEMORYREGIONNAMERULE_H
