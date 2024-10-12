#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "CInputSectionStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CInputSectionStatement::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;

    for (const auto& childEntry : this->ParsedContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }

    FuseVectors(allViolations, this->FileSelector()->AggregateViolation());
    FuseVectors(allViolations, this->Violations());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CInputSectionStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (!this->Violations().empty() ? "[ ** ] " : "[ OK ] ") +
            std::string("CInputSectionStatement") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content:";

    for (const auto subContent : this->ParsedContent())
    {
        content += "\n" + subContent->ToDebugInfo(depth + 4, linkerScriptFile);
    }

    return std::string(depth, ' ') + " - " + content;
}
