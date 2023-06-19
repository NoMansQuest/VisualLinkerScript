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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CNoDuplicateMemoryRegionNameRule()
        {}
    };
}


#endif // CNODUPLICATEMEMORYREGIONNAMERULE_H
