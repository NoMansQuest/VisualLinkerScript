#ifndef CINCLUDEDFILESAREPRESENTRULE_H
#define CINCLUDEDFILESAREPRESENTRULE_H

#include "../IDrcRuleBase.h"

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CInputFilesAreFoundRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CInputFilesAreFoundRule() = default;

        /// @copydoc
        std::string DrcRuleTitle() override
        {
            return "Input Files Are Found Rule";
        }

        /// @copydoc
        LinqVector<CViolationBase> PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) override;

        /// @brief Default destructor
        ~CInputFilesAreFoundRule() override = default;
    };
}

#endif // CINCLUDEDFILESAREPRESENTRULE_H
