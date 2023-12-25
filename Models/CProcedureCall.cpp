#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CProcedureCall.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CProcedureCall::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CProcedureCall") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content :";

    content += "\n" + this->AssignmentStatmeent().ToDebugInfo(depth + 4, linkerScriptFile);
    return content;
}
