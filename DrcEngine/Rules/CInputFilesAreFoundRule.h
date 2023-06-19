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
        virtual std::vector<std::shared_ptr<CDrcViolation>> PerformCheck(const CLinkerScriptFile& linkerScriptFile) override;

        /// @brief Default destructor
        ~CInputFilesAreFoundRule()
        {}
    };
}

#endif // CINPUTFILESAREFOUNDRULE_H
