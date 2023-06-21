#ifndef CINPUTFILESAREFOUNDRULE_H
#define CINPUTFILESAREFOUNDRULE_H

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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const std::vector<std::shared_ptr<CLinkerScriptFile>>& linkerScriptFiles) override;

        /// @brief Default destructor
        ~CInputFilesAreFoundRule()
        {}
    };
}

#endif // CINPUTFILESAREFOUNDRULE_H
