#include "CDirectivesDeclaredOnlyOnceRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CFunctionCall.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"

REGISTER_DRC_RULE(CDirectivesDeclaredOnlyOnceRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

std::vector<std::shared_ptr<CDrcViolation>> CDirectivesDeclaredOnlyOnceRule::PerformCheck(const std::vector<std::shared_ptr<CLinkerScriptFile>>& linkerScriptFiles) {
    std::vector<std::shared_ptr<CDrcViolation>> violations;

    std::vector<std::string> directives {
        "ENTRY",
        "OUTPUT",
        "OUTPUT_ARCH",
        "OUTPUT_FORMAT",
        "TARGET",
        "STARTUP",
        "SEARCH_DIR",
    };

    for (auto directive : directives) {
        auto foundDirectives = QueryObject<CFunctionCall>(
                    linkerScriptFiles,
                    [&](std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<CFunctionCall> ResolveEntryText) {
                        return StringEquals(linkerScriptFile->ResolveEntryText(ResolveEntryText->ProcedureName()), directive, true);
                    });

        if (foundDirectives.size() > 1) {
             violations.emplace_back(std::make_shared<CDrcViolation>(
                                     std::vector<std::shared_ptr<CLinkerScriptContentBase>>(),
                                     this->DrcRuleTitle(),
                                     "Directive is defined more than once.",
                                     nullptr,
                                     EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce,
                                     EDrcViolationSeverity::Error));
        }
    }

    return violations;
}
