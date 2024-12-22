#include "CNoDuplicateMemoryRegionNameRule.h"
#include "../../Models/CMemoryStatement.h"
#include "../../Helpers.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CCorrectiveAction.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "Models/CLinkerScriptFile.h"

REGISTER_DRC_RULE(CNoDuplicateMemoryRegionNameRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
LinqVector<CViolationBase> CNoDuplicateMemoryRegionNameRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    LinqVector<CViolationBase> violations;

    /*
    const auto foundMemoryStatements = linkerScriptFile->ParsedContent().OfType<CMemoryStatement>();

    for (const auto memoryStatementToInspect: foundMemoryStatements) {
        // Check if any other file has the name described
       LinqVector<CMemoryStatement> foundDuplicates { memoryStatementToInspect };
       auto inspectStatementName = linkerScriptFile->ResolveRawEntry(memoryStatementToInspect->NameEntry());

       for (const auto memoryStatementSecondLoop: foundMemoryStatements) {
            if (memoryStatementSecondLoop->StartPosition() == memoryStatementToInspect->StartPosition()) {
                continue;
            }

            auto secondLoopStatementName = linkerScriptFile->ResolveRawEntry(memoryStatementSecondLoop->NameEntry());

            if (StringEquals(inspectStatementName,secondLoopStatementName)){
                foundDuplicates.emplace_back(memoryStatementSecondLoop);
            }
        }

        if (foundDuplicates.size() == 1) {
            continue;
        }

        auto errorMessage = StringFormat("Memory statement names are expected to be unique, but multiple memory statements found named '{}'", inspectStatementName);        
        LinqVector<CDrcViolation> subitems;

        // NOTE: We skip index '0', as that's the 'memoryStatementToInspect' itself.
        for (auto index = 1; index < foundDuplicates.size(); index++)
        {
            auto subItemStatement = foundDuplicates[index];
            std::string subItemErrorMessage = "Memory statement with identical name defined here";
            LinqVector<CParsedContentBase> subItemStatements {
                std::dynamic_pointer_cast<CParsedContentBase>(subItemStatement)
            };

            subitems.emplace_back(std::make_shared<CDrcViolation>(
	            subItemStatements,
	            this->DrcRuleTitle(),
	            subItemErrorMessage,
	            subItemStatement->ObjectPath(),
	            LinqVector<CDrcViolation>(),
	            std::shared_ptr<CCorrectiveAction>(nullptr),
	            EDrcViolationCode::DuplicateNameForMemoryStatement,
                ESeverityCode::Error));
        }

        LinqVector<CParsedContentBase> memoryStatementsToInspect {
            std::dynamic_pointer_cast<CParsedContentBase>(memoryStatementToInspect)
        };

        auto violation = std::shared_ptr<CDrcViolation>(new CDrcViolation(
            memoryStatementsToInspect,
            this->DrcRuleTitle(),
            errorMessage,
            memoryStatementToInspect->ObjectPath(),
            std::move(subitems),
            std::shared_ptr<CCorrectiveAction>(nullptr),
            EDrcViolationCode::DuplicateNameForMemoryStatement,
            ESeverityCode::Error));

        violations.emplace_back(std::static_pointer_cast<CViolationBase>(violation));
    }
    */
    return violations;
}

