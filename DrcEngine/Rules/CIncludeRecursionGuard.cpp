#include <algorithm>

#include "CIncludeRecursionGuard.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CIncludeCommand.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"

REGISTER_DRC_RULE(CIncludeRecursionGuard)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

#ifdef Q_OS_WINDOWS
#define IGNORE_CASE_WHEN_COMPARING_PATHS true
#else
#define IGNORE_CASE_WHEN_COMPARING_PATHS false
#endif

typedef std::unordered_map<std::shared_ptr<CLinkerScriptFile>, std::shared_ptr<CIncludeCommand>> RecursionMap;

/// @brief Recursively checks the given scope to see if @see {sourceFile} is re-observed in the 'include' chain
bool RecursiveCheck(const SharedPtrVector<CLinkerScriptFile>& scope,
                    std::shared_ptr<CLinkerScriptFile> fileToCheck,
                    std::shared_ptr<CLinkerScriptFile> subjectOfInterest,
                    RecursionMap& recursionMap);

std::vector<std::shared_ptr<CDrcViolation>> CIncludeRecursionGuard::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {

    std::vector<std::shared_ptr<CDrcViolation>> violations;

    auto foundIncludeCommands = QueryObject<CIncludeCommand>(linkerScriptFiles);

    for (auto file: linkerScriptFiles) {
        // Recursion list
        std::vector<std::string> visitedFile { file->AbsoluteFilePath() };

        for (auto includeCommand: QueryObject<CIncludeCommand>(file)) {

            // Check if file exists
            auto linkerScriptFile = includeCommand.first;
            auto targetFile = linkerScriptFile->ResolveEntryText(*includeCommand.second);
            std::shared_ptr<CLinkerScriptFile> fileToCheckRecursively = nullptr;
            RecursionMap recursionMap;

            // Check if any other file has the name described
            for (auto hoveringFile: linkerScriptFiles) {
                if (hoveringFile->AbsoluteFilePath().compare(includeCommand.first->AbsoluteFilePath()) == 0) {
                    continue; // The file isn not cross-checked against itself
                }

                if (hoveringFile->FileName().compare(targetFile) == 0) {
                    fileToCheckRecursively = hoveringFile;
                    break;
                }
            }

            // If target file was found, no violation is needed
            if (fileToCheckRecursively != nullptr) {
                if (RecursiveCheck(linkerScriptFiles, fileToCheckRecursively, file, recursionMap)){

                    auto errorMessage = StringFormat("Recursion detected in file '{}' when included '{}' on line '{}':\n",
                                                     file->FileName(),
                                                     targetFile,
                                                     includeCommand.second->IncludeFileEntry().StartLineNumber());

                    recursionMap.reserve(recursionMap.size());

                    // Include the content of the chain
                    for (auto chainEntry: recursionMap)
                    {
                        auto messageToAppend = StringFormat("In file '{}' when included '{}' on line '{}'\n",
                                                            chainEntry.first->FileName(),
                                                            chainEntry.first->ResolveEntryText(chainEntry.second->IncludeFileEntry()),
                                                            chainEntry.second->IncludeFileEntry().StartLineNumber());
                        errorMessage.append(messageToAppend);
                    }

                    violations.emplace_back(std::make_shared<CDrcViolation>(
                                            std::vector<std::shared_ptr<CLinkerScriptContentBase>> { std::dynamic_pointer_cast<CLinkerScriptContentBase>(includeCommand.second) },
                                            this->DrcRuleTitle(),
                                            errorMessage,
                                            nullptr,
                                            EDrcViolationCode::RecursiveIncludeChainDetected,
                                            EDrcViolationSeverity::Error));
                }

                continue;
            }

            // Include file was not found
            auto errorMessage = StringFormat("Included files are expected to be present, but '{}' was not found", targetFile);
            violations.emplace_back(std::make_shared<CDrcViolation>(
                                    std::vector<std::shared_ptr<CLinkerScriptContentBase>> { std::dynamic_pointer_cast<CLinkerScriptContentBase>(includeCommand.second) },
                                    this->DrcRuleTitle(),
                                    errorMessage,
                                    nullptr,
                                    EDrcViolationCode::IncludedFilesArePresentRule,
                                    EDrcViolationSeverity::Error));
        }

    }

    return violations;
}


bool RecursiveCheck(const SharedPtrVector<CLinkerScriptFile>& scope,
                    std::shared_ptr<CLinkerScriptFile> fileToCheck,
                    std::shared_ptr<CLinkerScriptFile> subjectOfInterest,
                    RecursionMap& recursionMap)
{
    auto includeCommands  = QueryObject<CIncludeCommand>(fileToCheck);

    for (auto includeCommand: includeCommands) {
        auto fileToInclude = fileToCheck->ResolveEntryText(includeCommand.second->IncludeFileEntry());

        if (StringEquals(fileToInclude, subjectOfInterest->FileName(), IGNORE_CASE_WHEN_COMPARING_PATHS)) {
            recursionMap.insert_or_assign(fileToCheck, includeCommand);
            return true;
        }

        // Find the include file in the scope
        auto foundFile = std::find_if(
                    scope.cbegin(),
                    scope.cend(),
                    [=, &subjectOfInterest](const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile){
                        return StringEquals(subjectOfInterest->FileName(),
                                            linkerScriptFile->FileName(),
                                            IGNORE_CASE_WHEN_COMPARING_PATHS);
                    });

        if (foundFile == scope.cend() || !RecursiveCheck(scope, *foundFile, subjectOfInterest, recursionMap)) {
            continue;
        }

        recursionMap.insert_or_assign(fileToCheck, includeCommand);
        return true;
    }

    return false;
}

