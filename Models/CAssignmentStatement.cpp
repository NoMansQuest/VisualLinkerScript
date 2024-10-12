#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CAssignmentStatement.h"
#include "CLinkerScriptFile.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CAssignmentStatement::AggregateViolation() const
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
const std::string CAssignmentStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CAssignmentStatement") +
            " @pos " + std::to_string(this->StartPosition()) + " @line " + std::to_string(this->RawEntries()[0].StartLineNumber());

    content += "\n" + std::string(depth, ' ') + "  LValue: " + linkerScriptFile.ResolveRawEntry(this->LValueEntry());
    content += "\n" + std::string(depth, ' ') + "  Operator: '" + linkerScriptFile.ResolveRawEntry(this->AssignmentOperator()) + "'";
    content += "\n" + std::string(depth, ' ') + "  Content:";

    for (const auto &subContent : this->ParsedContent())
    {
        content += "\n" + subContent->ToDebugInfo(depth + 4, linkerScriptFile);
    }        

    return content;
}
