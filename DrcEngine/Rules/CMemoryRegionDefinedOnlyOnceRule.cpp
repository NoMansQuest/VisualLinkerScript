#include "CMemoryRegionDefinedOnlyOnceRule.h"
#include "../../Models/CMemoryRegion.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"

REGISTER_DRC_RULE(CMemoryRegionDefinedOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::DrcEngine::Rules;

SharedPtrVector<CViolationBase> CMemoryRegionDefinedOnlyOnceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile) {
    SharedPtrVector<CViolationBase> violations;

   auto foundSectionsRegion = QueryObject<CMemoryRegion>(linkerScriptFile, nullptr, true);

   for (const auto sectionsRegionResult: foundSectionsRegion) 
   {
       auto errorMessage = "'SECTIONS' region could only be defined once in linker-script file.";
       auto violation = std::shared_ptr<CDrcViolation>(new CDrcViolation(
           SharedPtrVector<CLinkerScriptContentBase>{
	           std::dynamic_pointer_cast<CLinkerScriptContentBase>(sectionsRegionResult->Result())
           },
           this->DrcRuleTitle(),
           errorMessage,
           sectionsRegionResult->Result()->ObjectPath(),
           {},
           nullptr,
           EDrcViolationCode::MemoryRegionDefinedMultipleTimes,
           ESeverityCode::Error));

       violations.emplace_back(std::static_pointer_cast<CViolationBase>(violation));
   }

    return violations;
}
