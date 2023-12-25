#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <stdint.h>

#include "CLinkerScriptFile.h"
#include "CFunctionCall.h"
#include "../ParsingEngine/CParserViolation.h"
#include "../DrcEngine/CDrcViolation.h"
#include "../DrcEngine/EDrcViolationCode.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

/// @brief Produces debug information on what this object represents.
const std::string CFunctionCall::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::stringstream contentStream;
    contentStream << (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ")
            << std::string("CFunctionCall ")
            << "[ " << linkerScriptFile.ResolveEntryText(this->m_functionName) << " ]"
            << " @pos "
            << std::to_string(this->StartPosition())
            << " -- content :";

    for (const auto &subContent : this->ParsedContent())
    {
        contentStream << "\n" << subContent->ToDebugInfo(depth + 4, linkerScriptFile);
    }

    for (auto violation : this->Violations())
    {
        switch (violation->Type())
        {
            case EViolationType::ParserViolation:
            {                
                auto converted = std::dynamic_pointer_cast<CParserViolation>(violation);
                auto firstEntryInViolation = converted->InvoledEntries().size() > 0 ? converted->InvoledEntries()[0] : CRawEntry();
                contentStream << std::string(depth, ' ')
                              << " - Violation Code: "
                              << MapParserViolationCodeToString(converted->Code());
                if (firstEntryInViolation.IsPresent())
                {
                    contentStream << " @line: "
                                  << firstEntryInViolation.StartLineNumber()
                                  << " @post: "
                                  << firstEntryInViolation.StartPosition()
                                  << " content: "
                                  << linkerScriptFile.ResolveEntryText(firstEntryInViolation);
                }
                contentStream  << "\n";
                break;
            }

            case EViolationType::DrcViolation:
            {
                auto converted = std::dynamic_pointer_cast<CDrcViolation>(violation);
                contentStream << std::string(depth, ' ') << " - Violation Code: " << (uint32_t)converted->Code() << "\n";
                break;
            }

            default:
                throw std::invalid_argument("violation::Type out of range.");

        }
    }

    return std::string(depth, ' ') + " - " + contentStream.str();
}
