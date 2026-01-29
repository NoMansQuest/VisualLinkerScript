#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "Models/CAssignmentProcedureStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const LinqVector<CViolationBase> CAssignmentProcedureStatement::AggregateViolation() const
{
    LinqVector<CViolationBase> allViolations;

    if (this->AssignmentStatement() != nullptr)
    {
        FuseVectors(allViolations, this->AssignmentStatement()->AggregateViolation());
    }

    for (const auto& childEntry : this->ParsedContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }

	FuseVectors(allViolations, this->Violations());
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
