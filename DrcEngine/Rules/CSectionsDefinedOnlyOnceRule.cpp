#include "CSectionsDefinedOnlyOnceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

REGISTER_DRC_RULE(CSectionsDefinedOnlyOnceRule)

using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript;

using namespace VisualLinkerScript::DrcEngine::Rules;

SharedPtrVector<CViolationBase> CSectionsDefinedOnlyOnceRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CViolationBase> violations;

    for (auto linkerScriptFile: linkerScriptFiles) {
       auto foundSectionsRegion = QueryObject<CSectionsRegion>(linkerScriptFiles, nullptr, true);

        // Check if any other file has the name described
       if (foundSectionsRegion.size() <= 1) {
           continue;
       }

       for (auto sectionsRegionResult: foundSectionsRegion) {
           auto errorMessage = "'SECTIONS' region could only be defined once in linker-script file.";
           violations.emplace_back(std::shared_ptr<CViolationBase>(new CDrcViolation(
                                       SharedPtrVector<CLinkerScriptContentBase> { std::dynamic_pointer_cast<CLinkerScriptContentBase>(sectionsRegionResult->Result()) },
                                       this->DrcRuleTitle(),
                                       errorMessage,
                                       sectionsRegionResult->Result()->ObjectPath(),
                                       {},
                                       nullptr,
                                       EDrcViolationCode::SectionsRegionDefinedMultipleTimes,
                                       EDrcViolationSeverity::Error)));
       }
    }

    return violations;
}

