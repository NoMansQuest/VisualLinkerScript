#include <vector>
#include <memory>
#include <string>

#include "CExpression.h"
#include "CLinkerScriptFile.h"
#include "../ParsingEngine/CParserViolation.h"
#include "../DrcEngine/CDrcViolation.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

const SharedPtrVector<CViolationBase> CExpression::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    for (const auto& childEntry : this->Composition())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }
    FuseVectors(allViolations, this->Violations());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CExpression::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CExpression") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content :";

    for (auto subContent : this->Composition())
    {
        content += "\n" + subContent->ToDebugInfo(depth + 4, linkerScriptFile);
    }

    if (this->Violations().size() > 0)
    {
        content += "\n";
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
                        MapParserViolationToCode(converted->Code()) +
                        " @line: " + std::to_string(converted->InvoledEntries()[0].StartLineNumber()) +
                        " @post: " + std::to_string(converted->InvoledEntries()[0].StartPosition()) +
                        " content: " + ((converted->InvoledEntries().size() > 0) ? linkerScriptFile.ResolveRawEntry(converted->InvoledEntries()[0]) : "<NONE>") +
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
                        " content: " + ((converted->InvolvedElements().size() > 0) ? linkerScriptFile.ResolveParsedContent(*converted->InvolvedElements()[0]) : "<NONE>") +
                        "\n";
                break;
            }

            default:
                throw std::invalid_argument("violation::Type out of range.");
        }
    }

    return std::string(depth, ' ') + " - " + content;
}
