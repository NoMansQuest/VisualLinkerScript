#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CAssignmentProcedureStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CAssignmentProcedureStatement::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    if (this->AssignmentStatement() != nullptr)
    {
        allViolations.insert(
            allViolations.end(), 
            this->AssignmentStatement()->AggregateViolation().cbegin(),
            this->AssignmentStatement()->AggregateViolation().cend());
    }
    for (const auto& childEntry : this->ParsedContent())
    {
        allViolations.insert(
            allViolations.end(),
            childEntry->AggregateViolation().cbegin(),
            childEntry->AggregateViolation().cend());
    }
    allViolations.insert(allViolations.end(), this->Violations().begin(), this->Violations().end());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

const std::string CAssignmentProcedureStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CAssignmentProcedureStatement") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content:";

    for (const auto& subContent : this->ParsedContent())
    {
        content += "\n" + subContent->ToDebugInfo(depth + 4, linkerScriptFile);
    }

    return std::string(depth, ' ') + " - " + content;
}
