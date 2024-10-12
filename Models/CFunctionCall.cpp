#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <cstdint>

#include "CLinkerScriptFile.h"
#include "CFunctionCall.h"
#include "../ParsingEngine/CParserViolation.h"
#include "../DrcEngine/CDrcViolation.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

const SharedPtrVector<CViolationBase> CFunctionCall::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    for (const auto& childEntry : this->ParsedContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }
    FuseVectors(allViolations, this->Violations());    
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CFunctionCall::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::stringstream contentStream;
    contentStream << (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ")
            << std::string("CFunctionCall ")
            << "[ " << linkerScriptFile.ResolveRawEntry(this->m_functionName) << " ]"
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
                              << MapParserViolationToCode(converted->Code());
                if (firstEntryInViolation.IsPresent())
                {
                    contentStream << " @line: "
                                  << firstEntryInViolation.StartLineNumber()
                                  << " @post: "
                                  << firstEntryInViolation.StartPosition()
                                  << " content: "
                                  << linkerScriptFile.ResolveRawEntry(firstEntryInViolation);
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
