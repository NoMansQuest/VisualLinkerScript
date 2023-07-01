#ifndef CNODUPLICATEMEMORYREGIONNAMERULE_H
#define CNODUPLICATEMEMORYREGIONNAMERULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CNoDuplicateMemoryRegionNameRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CNoDuplicateMemoryRegionNameRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "No Duplicate Memory Region Name Rule";
        }

        /// @copydoc
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CNoDuplicateMemoryRegionNameRule()
        {}
    };
}


#endif // CNODUPLICATEMEMORYREGIONNAMERULE_H
