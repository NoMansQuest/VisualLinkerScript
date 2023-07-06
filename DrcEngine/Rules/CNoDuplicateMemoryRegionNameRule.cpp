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

std::vector<std::shared_ptr<CDrcViolation>> CNoDuplicateMemoryRegionNameRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    std::vector<std::shared_ptr<CDrcViolation>> violations;

    auto foundMemoryStatements = QueryObject<CMemoryStatement>(linkerScriptFiles, nullptr, true);

    for (auto memoryStatementToInspect: foundMemoryStatements) {
        // Check if any other file has the name described
       SharedPtrVector<CMemoryStatement> foundDuplicates { memoryStatementToInspect };
       auto inpsectStatementName = memoryStatementToInspect->ParentLinkerScriptFile()->ResolveEntryText(memoryStatementToInspect->NameEntry());

       for (auto memoryStatementSecondLoop: foundMemoryStatements) {
            if (memoryStatementSecondLoop->StartPosition() == memoryStatementToInspect->StartPosition()) {
                continue;
            }

            auto secondLoopStatementName = memoryStatementSecondLoop->ParentLinkerScriptFile()->ResolveEntryText(memoryStatementSecondLoop->NameEntry());

            if (StringEquals(inpsectStatementName,secondLoopStatementName)){
                foundDuplicates.emplace_back(memoryStatementSecondLoop);
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
            auto subItemErrorMessage = "Memory statement with identical name defined here";
            subitems.emplace_back(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                                        SharedPtrVector<CLinkerScriptContentBase> { std::dynamic_pointer_cast<CLinkerScriptContentBase>(subItemStatement) },
                                        this->DrcRuleTitle(),
                                        subItemErrorMessage,
                                        subItemStatement->ObjectPath(),
                                        {},
                                        nullptr,
                                        EDrcViolationCode::DuplicateNameForMemoryStatement,
                                        EDrcViolationSeverity::Error)));
        }

        violations.emplace_back(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                                    SharedPtrVector<CLinkerScriptContentBase> { std::dynamic_pointer_cast<CLinkerScriptContentBase>(memoryStatementToInspect) },
                                    this->DrcRuleTitle(),
                                    errorMessage,
                                    memoryStatementToInspect->ObjectPath(),
                                    std::move(subitems),
                                    nullptr,
                                    EDrcViolationCode::DuplicateNameForMemoryStatement,
                                    EDrcViolationSeverity::Error)));
    }

    return violations;
}

