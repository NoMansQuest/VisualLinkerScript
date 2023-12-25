#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CAssignmentProcedureStatement.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CAssignmentProcedureStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CAssignmentProcedureStatement") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content:";

    for (auto subContent : this->ParsedContent())
    {
        content += "\n" + subContent->ToDebugInfo(depth + 4, linkerScriptFile);
    }

    return std::string(depth, ' ') + " - " + content;
}
