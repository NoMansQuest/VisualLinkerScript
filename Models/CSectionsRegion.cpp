#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "CSectionsRegion.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CSectionsRegion::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    for (const auto& childEntry : this->Entries())
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
const std::string CSectionsRegion::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CSectionsRegion") +
            " @pos " + std::to_string(this->StartPosition()) + "\n";

    content += std::string(depth, ' ') + " -- entries: \n";
    for (auto subContent : this->Entries())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return content;
}
