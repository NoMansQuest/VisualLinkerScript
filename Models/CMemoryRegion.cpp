#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "CMemoryRegion.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CMemoryRegion::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    for (const auto& childEntry : this->Statements())
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
const std::string CMemoryRegion::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CMemoryRegion") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content : \n";

    for (auto subContent : this->Statements())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return std::string(depth, ' ') + " - " + content;
}
