#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionOutputCommand.h"
#include "CLinkerScriptFile.h"
#include "../ParsingEngine/CParserViolation.h"
#include "../DrcEngine/CDrcViolation.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

const SharedPtrVector<CViolationBase> CSectionOutputCommand::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;

    for (const auto& childEntry : this->PreColonContent())
    {
        allViolations.insert(
            allViolations.end(),
            childEntry->AggregateViolation().cbegin(),
            childEntry->AggregateViolation().cend());
    }

    for (const auto& childEntry : this->PostColonContent())
    {
        allViolations.insert(
            allViolations.end(),
            childEntry->AggregateViolation().cbegin(),
            childEntry->AggregateViolation().cend());
    }

    for (const auto& childEntry : this->InnerContent())
    {
        allViolations.insert(
            allViolations.end(),
            childEntry->AggregateViolation().cbegin(),
            childEntry->AggregateViolation().cend());
    }

    for (const auto& childEntry : this->EndingContent())
    {
        allViolations.insert(
            allViolations.end(),
            childEntry->AggregateViolation().cbegin(),
            childEntry->AggregateViolation().cend());
    }

    allViolations.insert(allViolations.end(), this->Violations().begin(), this->Violations().end());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

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

    for (const auto& subContent : this->PreColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Post-colon content : \n";

    for (const auto& subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- PreColon content : \n";

    for (const auto& subContent : this->PreColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- PostColon content : \n";

    for (const auto& subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Inner content : \n";

    for (const auto& subContent : this->InnerContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Ending content : \n";

    for (const auto& subContent : this->EndingContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    for (const auto& violation : this->Violations())
    {
        switch (violation->Type())
        {
            case EViolationType::ParserViolation:
            {
                auto converted = std::dynamic_pointer_cast<CParserViolation>(violation);
                content += std::string(depth, ' ') +
                        " - Violation Code: " +
                        MapParserViolationToCode(converted->Code()) +
                        " @line: " + std::to_string(converted->InvoledEntries()[0].StartLineNumber()) +
                        " @post: " + std::to_string(converted->InvoledEntries()[0].StartPosition()) +
                        " content: " + ((!converted->InvoledEntries().empty()) ? linkerScriptFile.ResolveRawEntry(converted->InvoledEntries()[0]) : "<NONE>") +
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
                        " content: " + ((!converted->InvolvedElements().empty()) ? linkerScriptFile.ResolveParsedContent(*converted->InvolvedElements()[0]) : "<NONE>") +
                        "\n";
                break;
            }

            default:
                throw std::invalid_argument("violation::Type out of range.");
        }
    }

    return content;
}
