#include "CIncludedFilesArePresentRule.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CIncludeCommand.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "DrcEngine/EDrcViolationSeverity.h"

REGISTER_DRC_RULE(CInputFilesAreFoundRule)

using namespace VisualLinkerScript::DrcEngine::Rules;

SharedPtrVector<CViolationBase> CInputFilesAreFoundRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CViolationBase> violations;

    const auto foundIncludeCommands = QueryObject<CIncludeCommand>(linkerScriptFiles);

    for (const auto includeCommandResult: foundIncludeCommands) {
        // Check if file exists        
        auto targetFile = includeCommandResult->LinkerScriptFile()->ResolveEntryText(includeCommandResult->Result()->IncludeFileEntry());

        // Check if any other file has the name described
        auto targetFileFound = false;
        for (const auto hoveringFile: linkerScriptFiles) {
            if (hoveringFile->AbsoluteFilePath().compare(includeCommandResult->LinkerScriptFile()->AbsoluteFilePath()) == 0) {
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
        violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
	        std::vector{
		        std::dynamic_pointer_cast<CLinkerScriptContentBase>(includeCommandResult->Result())
	        },
	        this->DrcRuleTitle(),
	        errorMessage,
	        EDrcViolationCode::IncludedFilesArePresentRule,
	        EDrcViolationSeverity::Error))));
    }

    return violations;
}

