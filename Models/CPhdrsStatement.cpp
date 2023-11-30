#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CPhdrsStatement.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CPhdrsStatement::ToDebugInfo(uint32_t depth)
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition()) +
            " -- content : \n";

    content += this->AtAddressFunction()->ToDebugInfo(depth + 4) + "\n";
    content += this->FlagsFunction()->ToDebugInfo(depth + 4) + "\n";

    return std::string(depth, ' ') + " - " + content;
}