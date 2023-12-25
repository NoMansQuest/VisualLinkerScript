#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CPhdrsStatement.h"

using namespace VisualLinkerScript::Models;

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
