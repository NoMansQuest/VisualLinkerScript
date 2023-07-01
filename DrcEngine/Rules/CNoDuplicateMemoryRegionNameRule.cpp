#include "CNoDuplicateMemoryRegionNameRule.h"
#include "../../Models/CMemoryStatement.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CNoDuplicateMemoryRegionNameRule)

using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;

std::vector<std::shared_ptr<CDrcViolation>> CNoDuplicateMemoryRegionNameRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    std::vector<std::shared_ptr<CDrcViolation>> violations;

    auto foundMemoryStatements = QueryObject<CMemoryStatement>(linkerScriptFiles, nullptr, true);

    for (auto memoryStatementToInspect: foundMemoryStatements) {
        // Check if any other file has the name described
        SharedPtrVector<CMemoryStatement> foundDuplicates;

        for (auto memoryStatementSecondLoop: foundMemoryStatements) {
            if (memoryStatementSecondLoop.second->StartPosition() == memoryStatementToInspect->StartPosition()) {
                continue;
            }

            if (hoveringFile->FileName().compare(targetFile) == 0) {
                targetFileFound = true;
                break;
            }
        }

        // If target file was found, no violation is needed
        if (targetFileFound) {
            continue;
        }

        auto errorMessage = StringFormat("Included files are expected to be present, but '{}' was not found", targetFile);
        violations.emplace_back(std::make_shared<CDrcViolation>(
                                std::vector<std::shared_ptr<CLinkerScriptContentBase>> { std::dynamic_pointer_cast<CLinkerScriptContentBase>(includeCommand.second) },
                                this->DrcRuleTitle(),
                                errorMessage,
                                nullptr,
                                EDrcViolationCode::IncludedFilesArePresentRule,
                                EDrcViolationSeverity::Error));
    }

    return violations;
}

