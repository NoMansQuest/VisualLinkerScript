#include "CEntryIsDefinedRule.h"
#include "../DrcCommons.h"
#include "../CDrcManager.h"
#include "../IDrcRuleBase.h"

#include "../../Models/CLinkerScriptFile.h"
#include "../../Models/CFunctionCall.h"
#include "../../Helpers.h"
#include "../../QueryEngine/QueryCenter.h"

REGISTER_DRC_RULE(CEntryIsDefinedRule)

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

SharedPtrVector<CViolationBase> CEntryIsDefinedRule::PerformCheck(const SharedPtrVector<CLinkerScriptFile>& linkerScriptFiles) {
    SharedPtrVector<CViolationBase> violations;

    auto foundDirectives = QueryObject<CFunctionCall>(
                linkerScriptFiles,
                [&](std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<CFunctionCall> ResolveEntryText) {
                    return StringEquals(linkerScriptFile->ResolveEntryText(ResolveEntryText->FunctionName()), "ENTRY", true);
                });

    if (foundDirectives.size() == 0) {
         violations.emplace_back(std::shared_ptr<CViolationBase>(new CDrcViolation(
                                 std::vector<std::shared_ptr<CLinkerScriptContentBase>>(),
                                 this->DrcRuleTitle(),
                                 "Directive is defined more than once.",
                                 EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce,
                                 EDrcViolationSeverity::Error)));
    }

    return violations;
}



