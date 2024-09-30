#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "CProcedureCall.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CProcedureCall::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    allViolations.insert(allViolations.end(), this->AssignmentStatement().Violations().cbegin(), this->AssignmentStatement().Violations().cend());
    allViolations.insert(allViolations.end(), this->Violations().begin(), this->Violations().end());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CProcedureCall::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CProcedureCall") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content :";

    content += "\n" + this->AssignmentStatement().ToDebugInfo(depth + 4, linkerScriptFile);
    return content;
}
