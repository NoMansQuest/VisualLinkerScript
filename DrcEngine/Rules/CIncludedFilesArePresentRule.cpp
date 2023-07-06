#include "CIncludedFilesArePresentRule.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CIncludeCommand.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"

REGISTER_DRC_RULE(CInputFilesAreFoundRule)

using namespace VisualLinkerScript::DrcEngine::Rules;

std::vector<std::shared_ptr<CDrcViolation>> CInputFilesAreFoundRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    std::vector<std::shared_ptr<CDrcViolation>> violations;

    auto foundIncludeCommands = QueryObject<CIncludeCommand>(linkerScriptFiles);

    for (auto includeCommand: foundIncludeCommands) {
        // Check if file exists        
        auto targetFile = includeCommand->ParentLinkerScriptFile()->ResolveEntryText(includeCommand->IncludeFileEntry());

        // Check if any other file has the name described
        auto targetFileFound = false;
        for (auto hoveringFile: linkerScriptFiles) {
            if (hoveringFile->AbsoluteFilePath().compare(includeCommand.ParentLinkerScriptFile()->AbsoluteFilePath()) == 0) {
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

