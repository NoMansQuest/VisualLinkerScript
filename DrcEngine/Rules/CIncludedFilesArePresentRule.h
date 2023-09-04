#ifndef CINCLUDEDFILESAREPRESENTRULE_H
#define CINCLUDEDFILESAREPRESENTRULE_H

#include "../IDrcRuleBase.h"
#include <memory>

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CInputFilesAreFoundRule : public IDrcRuleBase
    {
    public:
        /// @brief Default constructor
        CInputFilesAreFoundRule()
        {}

        /// @copydoc
        virtual std::string DrcRuleTitle() override
        {
            return "Input Files Are Found Rule";
        }

        /// @copydoc
        virtual SharedPtrVector<CViolationBase> PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CInputFilesAreFoundRule()
        {}
    };
}

#endif // CINCLUDEDFILESAREPRESENTRULE_H
