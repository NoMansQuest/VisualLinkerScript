#include "DrcEngine/Rules/CMemoryRegionDefinedOnlyOnceRule.h"
#include "Models/CMemoryRegion.h"
#include "Helpers.h"
#include "DrcEngine/DrcCommons.h"
#include "DrcEngine/CDrcManager.h"
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
	
    auto foundMemoryRegions = linkerScriptFile->ParsedContent().OfType<CMemoryRegion>();
    if (foundMemoryRegions.size() > 1)
    {
	    for (const auto foundMemoryRegion : foundMemoryRegions)
	    {
	        auto errorMessage = "'MEMORY' region could only be defined at most once in the linker-script file.";
	        auto violation = std::shared_ptr<CDrcViolation>(new CDrcViolation(
	            LinqVector<CParsedContentBase>{
		            std::dynamic_pointer_cast<CParsedContentBase>(foundMemoryRegion)
	            },
	            this->DrcRuleTitle(),
	            errorMessage,
	            foundMemoryRegion->ObjectPath(),
	            {},
	            nullptr,
	            EDrcViolationCode::MemoryRegionDefinedMultipleTimes,
	            ESeverityCode::Error));
	    
	        violations.emplace_back(std::static_pointer_cast<CViolationBase>(violation));
	    }	    
    }

    return violations;
}
