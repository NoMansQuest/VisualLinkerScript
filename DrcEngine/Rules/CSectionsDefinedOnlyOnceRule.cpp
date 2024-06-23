#include "CSectionsDefinedOnlyOnceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "DrcEngine/EDrcViolationSeverity.h"

REGISTER_DRC_RULE(CSectionsDefinedOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;

SharedPtrVector<CViolationBase> CSectionsDefinedOnlyOnceRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CViolationBase> violations;

    for (auto linkerScriptFile: linkerScriptFiles) {
       auto foundSectionsRegion = QueryObject<CSectionsRegion>(linkerScriptFiles, nullptr, true);

        // Check if any other file has the name described
       if (foundSectionsRegion.size() <= 1) {
           continue;
       }

       for (const auto sectionsRegionResult: foundSectionsRegion) {
           auto errorMessage = "'SECTIONS' region could only be defined once in linker-script file.";
           violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>( new CDrcViolation(
	           SharedPtrVector<CLinkerScriptContentBase>{
		           std::dynamic_pointer_cast<CLinkerScriptContentBase>(sectionsRegionResult->Result())
	           },
	           this->DrcRuleTitle(),
	           errorMessage,
	           sectionsRegionResult->Result()->ObjectPath(),
	           {},
	           nullptr,
	           EDrcViolationCode::SectionsRegionDefinedMultipleTimes,
	           EDrcViolationSeverity::Error))));
       }
    }

    return violations;
}

