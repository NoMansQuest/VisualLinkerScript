#include "CNoDuplicateMemoryRegionNameRule.h"
#include "../../Models/CMemoryStatement.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "Models/Intervention/CIntervention.h"

REGISTER_DRC_RULE(CNoDuplicateMemoryRegionNameRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;

SharedPtrVector<CViolationBase> CNoDuplicateMemoryRegionNameRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    SharedPtrVector<CViolationBase> violations;

    const auto foundMemoryStatements = QueryObject<CMemoryStatement>(linkerScriptFile, nullptr, true);

    for (const auto memoryStatementToInspect: foundMemoryStatements) {
        // Check if any other file has the name described
       SharedPtrVector<CMemoryStatement> foundDuplicates { memoryStatementToInspect->Result() };
       auto inspectStatementName = memoryStatementToInspect->LinkerScriptFile()->ResolveRawEntry(memoryStatementToInspect->Result()->NameEntry());

       for (const auto memoryStatementSecondLoop: foundMemoryStatements) {
            if (memoryStatementSecondLoop->Result()->StartPosition() == memoryStatementToInspect->Result()->StartPosition()) {
                continue;
            }

            auto secondLoopStatementName = memoryStatementSecondLoop->LinkerScriptFile()->ResolveRawEntry(memoryStatementSecondLoop->Result()->NameEntry());

            if (StringEquals(inspectStatementName,secondLoopStatementName)){
                foundDuplicates.emplace_back(memoryStatementSecondLoop->Result());
            }
        }

        if (foundDuplicates.size() == 1) {
            continue;
        }

        auto errorMessage = StringFormat("Memory statement names are expected to be unique, but multiple memory statements found named '{}'", inspectStatementName);        
        SharedPtrVector<CDrcViolation> subitems;

        // NOTE: We skip index '0', as that's the 'memoryStatementToInspect' itself.
        for (auto index = 1; index < foundDuplicates.size(); index++)
        {
            auto subItemStatement = foundDuplicates[index];
            std::string subItemErrorMessage = "Memory statement with identical name defined here";
            SharedPtrVector<CParsedContentBase> subItemStatements {
                std::dynamic_pointer_cast<CParsedContentBase>(subItemStatement)
            };

            subitems.emplace_back(std::make_shared<CDrcViolation>(
	            subItemStatements,
	            this->DrcRuleTitle(),
	            subItemErrorMessage,
	            subItemStatement->ObjectPath(),
	            SharedPtrVector<CDrcViolation>(),
	            std::shared_ptr<Intervention::CIntervention>(nullptr),
	            EDrcViolationCode::DuplicateNameForMemoryStatement,
                ESeverityCode::Error));
        }

        SharedPtrVector<CParsedContentBase> memoryStatementsToInspect {
            std::dynamic_pointer_cast<CParsedContentBase>(memoryStatementToInspect->Result())
        };

        auto violation = std::shared_ptr<CDrcViolation>(new CDrcViolation(
            memoryStatementsToInspect,
            this->DrcRuleTitle(),
            errorMessage,
            memoryStatementToInspect->Result()->ObjectPath(),
            std::move(subitems),
            std::shared_ptr<Intervention::CIntervention>(nullptr),
            EDrcViolationCode::DuplicateNameForMemoryStatement,
            ESeverityCode::Error));

        violations.emplace_back(std::static_pointer_cast<CViolationBase>(violation));
    }

    return violations;
}

