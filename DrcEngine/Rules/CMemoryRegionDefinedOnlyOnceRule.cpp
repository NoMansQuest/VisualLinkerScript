#include "CMemoryRegionDefinedOnlyOnceRule.h"
#include "../../Models/CMemoryRegion.h"
#include "../../Helpers.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "Models/CLinkerScriptFile.h"

REGISTER_DRC_RULE(CMemoryRegionDefinedOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::DrcEngine::Rules;

LinqVector<CViolationBase> CMemoryRegionDefinedOnlyOnceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    LinqVector<CViolationBase> violations;

	/*
    auto foundSectionsRegion = linkerScriptFile->ParsedContent().OfType<CMemoryRegion>();
    
    for (const auto sectionsRegionResult: foundSectionsRegion) 
    {
        auto errorMessage = "'SECTIONS' region could only be defined once in linker-script file.";
        auto violation = std::shared_ptr<CDrcViolation>(new CDrcViolation(
            LinqVector<CParsedContentBase>{
	            std::dynamic_pointer_cast<CParsedContentBase>(sectionsRegionResult)
            },
            this->DrcRuleTitle(),
            errorMessage,
            sectionsRegionResult->ObjectPath(),
            {},
            nullptr,
            EDrcViolationCode::MemoryRegionDefinedMultipleTimes,
            ESeverityCode::Error));
    
        violations.emplace_back(std::static_pointer_cast<CViolationBase>(violation));
    }
    */
    return violations;
}
