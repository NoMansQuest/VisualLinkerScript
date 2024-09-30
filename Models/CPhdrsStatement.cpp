#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CPhdrsStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CPhdrsStatement::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    allViolations.insert(allViolations.end(), this->AtAddressFunction()->AggregateViolation().cbegin(), this->AtAddressFunction()->AggregateViolation().cend());
    allViolations.insert(allViolations.end(), this->FlagsFunction()->AggregateViolation().cbegin(), this->FlagsFunction()->AggregateViolation().cend());
    allViolations.insert(allViolations.end(), this->Violations().begin(), this->Violations().end());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CPhdrsStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CPhdrsStatement") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content :";

    content += "\n" + this->AtAddressFunction()->ToDebugInfo(depth + 4, linkerScriptFile);
    content += "\n" + this->FlagsFunction()->ToDebugInfo(depth + 4, linkerScriptFile);

    return content;
}
