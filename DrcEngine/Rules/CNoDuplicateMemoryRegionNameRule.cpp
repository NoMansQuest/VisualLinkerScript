#include "CNoDuplicateMemoryRegionNameRule.h"
#include "../../Models/CMemoryStatement.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../QueryEngine/CLinq.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CNoDuplicateMemoryRegionNameRule)

using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript;

SharedPtrVector<CViolationBase> CNoDuplicateMemoryRegionNameRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CViolationBase> violations;

    auto foundMemoryStatements = QueryObject<CMemoryStatement>(linkerScriptFiles, nullptr, true);

    for (auto memoryStatementToInspect: foundMemoryStatements) {
        // Check if any other file has the name described
       SharedPtrVector<CMemoryStatement> foundDuplicates { memoryStatementToInspect->Result() };
       auto inpsectStatementName = memoryStatementToInspect->LinkerScriptFile()->ResolveEntryText(memoryStatementToInspect->Result()->NameEntry());

       for (auto memoryStatementSecondLoop: foundMemoryStatements) {
            if (memoryStatementSecondLoop->Result()->StartPosition() == memoryStatementToInspect->Result()->StartPosition()) {
                continue;
            }

            auto secondLoopStatementName = memoryStatementSecondLoop->LinkerScriptFile()->ResolveEntryText(memoryStatementSecondLoop->Result()->NameEntry());

            if (StringEquals(inpsectStatementName,secondLoopStatementName)){
                foundDuplicates.emplace_back(memoryStatementSecondLoop->Result());
            }
        }

        if (foundDuplicates.size() == 1) {
            continue;
        }

        auto errorMessage = StringFormat("Memory statement names are expected to be unique, but multiple memory statements found named '{}'", inpsectStatementName);        
        SharedPtrVector<CDrcViolation> subitems;

        // NOTE: We skip index '0', as that's the 'memoryStatementToInspect' itself.
        for (auto index = 1; index < foundDuplicates.size(); index++)
        {
            auto subItemStatement = foundDuplicates[index];
            std::string subItemErrorMessage = "Memory statement with identical name defined here";
            SharedPtrVector<CLinkerScriptContentBase> subItemStatements {
                std::dynamic_pointer_cast<CLinkerScriptContentBase>(subItemStatement)
            };

            subitems.emplace_back(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                                        subItemStatements,
                                        this->DrcRuleTitle(),
                                        subItemErrorMessage,
                                        subItemStatement->ObjectPath(),
                                        SharedPtrVector<CDrcViolation>(),
                                        std::shared_ptr<CIntervention>(nullptr),
                                        EDrcViolationCode::DuplicateNameForMemoryStatement,
                                        EDrcViolationSeverity::Error)));
        }

        SharedPtrVector<CLinkerScriptContentBase> memoryStatementsToInspect {
            std::dynamic_pointer_cast<CLinkerScriptContentBase>(memoryStatementToInspect->Result())
        };

        violations.emplace_back(std::shared_ptr<CViolationBase>(new CDrcViolation(
                                    memoryStatementsToInspect,
                                    this->DrcRuleTitle(),
                                    errorMessage,
                                    memoryStatementToInspect->Result()->ObjectPath(),
                                    std::move(subitems),
                                    std::shared_ptr<CIntervention>(nullptr),
                                    EDrcViolationCode::DuplicateNameForMemoryStatement,
                                    EDrcViolationSeverity::Error)));
    }

    return violations;
}

