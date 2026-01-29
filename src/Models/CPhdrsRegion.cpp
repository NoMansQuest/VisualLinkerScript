#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "Models/CPhdrsRegion.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const LinqVector<CViolationBase> CPhdrsRegion::AggregateViolation() const
{
    LinqVector<CViolationBase> allViolations;
    for (const auto& childEntry : this->Statements())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }
    FuseVectors(allViolations, this->Violations());    
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CPhdrsRegion::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CPhdrsRegion") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content : \n";

    for (auto subContent : this->Statements())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return content;
}
