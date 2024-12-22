#include "CSectionsDefinedOnlyOnceRule.h"
#include "../../Models/CSectionsRegion.h"
#include "../../Helpers.h"

#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "DrcEngine/CDrcViolation.h"
#include "DrcEngine/EDrcViolationCode.h"
#include "Models/CLinkerScriptFile.h"

REGISTER_DRC_RULE(CSectionsDefinedOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Models;

LinqVector<CViolationBase> CSectionsDefinedOnlyOnceRule::PerformCheck(const std::shared_ptr<CLinkerScriptFile>& linkerScriptFile)
{
    LinqVector<CViolationBase> violations;

    /*
    auto foundSectionsRegion = linkerScriptFile->ParsedContent().OfType<CSectionsRegion>();
    if (foundSectionsRegion.size() > 1)
    {
        for (const auto sectionsRegionResult : foundSectionsRegion) {
            auto errorMessage = "'SECTIONS' region could only be defined once in linker-script file.";
            violations.emplace_back(std::static_pointer_cast<CViolationBase>(std::shared_ptr<CDrcViolation>(new CDrcViolation(
                LinqVector<CParsedContentBase>{
                std::dynamic_pointer_cast<CParsedContentBase>(sectionsRegionResult)
            },
                this->DrcRuleTitle(),
                errorMessage,
                sectionsRegionResult->ObjectPath(),
                {},
                nullptr,
                EDrcViolationCode::SectionsRegionDefinedMultipleTimes,
                ESeverityCode::Error))));
        }	    
    }
    */
    return violations;
}

