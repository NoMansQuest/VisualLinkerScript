#include "CSectionsDefinedOnlyOnceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../QueryEngine/QueryCenter.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"

REGISTER_DRC_RULE(CSectionsDefinedOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::QueryEngine;

SharedPtrVector<CViolationBase> CSectionsDefinedOnlyOnceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    SharedPtrVector<CViolationBase> violations;

    auto foundSectionsRegion = QueryObject<CSectionsRegion>(linkerScriptFile, nullptr, true);
    if (foundSectionsRegion.size() > 1)
    {
        for (const auto sectionsRegionResult : foundSectionsRegion) {
            auto errorMessage = "'SECTIONS' region could only be defined once in linker-script file.";
            violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                SharedPtrVector<CLinkerScriptContentBase>{
                std::dynamic_pointer_cast<CLinkerScriptContentBase>(sectionsRegionResult->Result())
            },
                this->DrcRuleTitle(),
                errorMessage,
                sectionsRegionResult->Result()->ObjectPath(),
                {},
                nullptr,
                EDrcViolationCode::SectionsRegionDefinedMultipleTimes,
                ESeverityCode::Error))));
        }	    
    }

    return violations;
}

