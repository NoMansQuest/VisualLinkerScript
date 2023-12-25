#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionOutputCommand.h"
#include "CLinkerScriptFile.h"
#include "../ParsingEngine/CParserViolation.h"
#include "../DrcEngine/CDrcViolation.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

/// @brief Produces debug information on what this object represents.
const std::string CSectionOutputCommand::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CSectionOutputCommand") +
            " @line " + std::to_string(this->RawEntries()[0].StartLineNumber()) +
            " @pos " + std::to_string(this->StartPosition()) + "\n";

    content += std::string(depth, ' ') + " -- Pre-colon content : \n";

    for (auto subContent : this->PreColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Post-colon content : \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- PreColon content : \n";

    for (auto subContent : this->PreColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- PostColon content : \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Inner content : \n";

    for (auto subContent : this->InnerContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Ending content : \n";

    for (auto subContent : this->EndingContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    for (auto violation : this->Violations())
    {
        switch (violation->Type())
        {
            case EViolationType::ParserViolation:
            {
                auto converted = std::dynamic_pointer_cast<CParserViolation>(violation);
                content += std::string(depth, ' ') +
                        " - Violation Code: " +
                        MapParserViolationCodeToString(converted->Code()) +
                        " @line: " + std::to_string(converted->InvoledEntries()[0].StartLineNumber()) +
                        " @post: " + std::to_string(converted->InvoledEntries()[0].StartPosition()) +
                        " content: " + ((converted->InvoledEntries().size() > 0) ? linkerScriptFile.ResolveEntryText(converted->InvoledEntries()[0]) : "<NONE>") +
                        "\n";
                break;
            }

            case EViolationType::DrcViolation:
            {
                auto converted = std::dynamic_pointer_cast<CDrcViolation>(violation);
                content += std::string(depth, ' ') +
                        " - Violation Code: " +
                        std::to_string((uint32_t)converted->Code()) +
                        " @line: " + std::to_string(converted->InvolvedElements()[0]->RawEntries()[0].StartLineNumber()) +
                        " @pos: " + std::to_string(converted->InvolvedElements()[0]->RawEntries()[0].StartLineNumber()) +
                        " content: " + ((converted->InvolvedElements().size() > 0) ? linkerScriptFile.ResolveEntryText(*converted->InvolvedElements()[0]) : "<NONE>") +
                        "\n";
                break;
            }

            default:
                throw std::invalid_argument("violation::Type out of range.");
        }
    }

    return content;
}
