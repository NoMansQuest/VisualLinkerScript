#include <algorithm>

#include "DrcEngine/Rules/CIncludeRecursionGuard.h"

#include "DrcEngine/DrcCommons.h"
#include "DrcEngine/CDrcManager.h"
#include "DrcEngine/IDrcRuleBase.h"

#include "Models/CLinkerScriptFile.h"
#include "Models/CIncludeCommand.h"
#include "Helpers.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"

REGISTER_DRC_RULE(CIncludeRecursionGuard)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

#if defined(COMPILING_FOR_WINDOWS)
#define IGNORE_CASE_WHEN_COMPARING_PATHS true
#elif defined(COMPILING_FOR_UNIX_BASED)
#define IGNORE_CASE_WHEN_COMPARING_PATHS false
#else
#error Neither 'COMPILING_FOR_WINDOWS' nor 'COMPILING_FOR_UNIX_BASED' were set.
#endif

typedef std::unordered_map<std::shared_ptr<CLinkerScriptFile>, std::shared_ptr<CIncludeCommand>> RecursionMap;

/// @brief Recursively checks the given scope to see if @see {sourceFile} is re-observed in the 'include' chain
bool RecursiveCheck(const LinqVector<CLinkerScriptFile>& scope,
                    const std::shared_ptr<CLinkerScriptFile>& fileToCheck,
                    std::shared_ptr<CLinkerScriptFile> subjectOfInterest,
                    RecursionMap& recursionMap);

LinqVector<CViolationBase> CIncludeRecursionGuard::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    LinqVector<CViolationBase> violations;

    /*
    auto foundIncludeCommands = linkerScriptFile->ParsedContent().OfType<CIncludeCommand>();

    for (auto file: linkerScriptFiles) {
        // Recursion list
        std::vector<std::string> visitedFile { file->AbsoluteFilePath() };

        for (auto includeCommandResult: QueryObject<CIncludeCommand>(file)) {

            // Check if file exists            
            auto targetFile = includeCommandResult->LinkerScriptFile()->ResolveParsedContent(includeCommandResult->Result()->IncludeFileEntry());
            std::shared_ptr<CLinkerScriptFile> fileToCheckRecursively = nullptr;
            RecursionMap recursionMap;

            // Check if any other file has the name described
            for (auto hoveringFile: linkerScriptFiles) {
                if (hoveringFile->AbsoluteFilePath().compare(includeCommandResult->LinkerScriptFile()->AbsoluteFilePath()) == 0) {
                    continue; // The file isn't cross-checked against itself
                }

                if (hoveringFile->FileName().compare(targetFile) == 0) {
                    fileToCheckRecursively = hoveringFile;
                    break;
                }
            }

            // If target file was found, no violation is needed
            if (fileToCheckRecursively != nullptr) {
                if (RecursiveCheck(linkerScriptFiles, fileToCheckRecursively, file, recursionMap)) {
                    auto errorMessage = StringFormat("Recursion detected in file '{}' when included '{}' on line '{}':\n",
                                                     file->FileName(),
                                                     targetFile,
                                                     includeCommandResult->Result()->IncludeFileEntry().StartLineNumber());

                    recursionMap.reserve(recursionMap.size());

                    // Include the content of the chain
                    for (auto chainEntry: recursionMap)
                    {
                        auto messageToAppend = StringFormat("In file '{}' when included '{}' on line '{}'\n",
                                                            chainEntry.first->FileName(),
                                                            chainEntry.first->ResolveParsedContent(chainEntry.second->IncludeFileEntry()),
                                                            chainEntry.second->IncludeFileEntry().StartLineNumber());
                        errorMessage.append(messageToAppend);
                    }

                    violations.emplace_back(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                                            std::vector { std::dynamic_pointer_cast<CParsedContentBase>(includeCommandResult->Result()) },
                                            this->DrcRuleTitle(),
                                            errorMessage,
                                            EDrcViolationCode::RecursiveIncludeChainDetected,
                                            EDrcViolationSeverity::Error)));
                }

                continue;
            }

            // Include file was not found
            auto errorMessage = StringFormat("Included files are expected to be present, but '{}' was not found", targetFile);
            violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
	            LinqVector<CParsedContentBase>{
		            std::dynamic_pointer_cast<CParsedContentBase>(includeCommandResult->Result())
	            },
	            this->DrcRuleTitle(),
	            errorMessage,
	            EDrcViolationCode::IncludedFilesArePresentRule,
	            EDrcViolationSeverity::Error))));
        }
    }
    */

    // TODO: Re-enable when concept in place.

    return violations;
}

/*
bool RecursiveCheck(const LinqVector<CLinkerScriptFile>& scope,
                    const std::shared_ptr<CLinkerScriptFile>& fileToCheck,
                    std::shared_ptr<CLinkerScriptFile> subjectOfInterest,
                    RecursionMap& recursionMap)
{
	const auto includeCommands  = fileToCheck->ParsedContent().OfType<CIncludeCommand>();

    for (const auto includeCommandResult: includeCommands) 
    {
	    if (auto fileToInclude = fileToCheck->ResolveRawEntry(includeCommandResult->IncludeFileEntry());
            StringEquals(fileToInclude, subjectOfInterest->FileName(),IGNORE_CASE_WHEN_COMPARING_PATHS)) 
        {
            recursionMap.insert_or_assign(fileToCheck, includeCommandResult);
            return true;
        }

        // Find the include file in the scope
        if (auto foundFile = std::find_if(
	        scope.cbegin(),
	        scope.cend(),
	        [=, &subjectOfInterest](const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile){
		        return StringEquals(subjectOfInterest->FileName(),
		                            linkerScriptFile->FileName(),
		                            IGNORE_CASE_WHEN_COMPARING_PATHS);
	        }); foundFile == scope.cend() || !RecursiveCheck(scope, *foundFile, subjectOfInterest, recursionMap)) {
            continue;
        }

        recursionMap.insert_or_assign(fileToCheck, includeCommandResult);
        return true;
    }

    return false;
}
*/
