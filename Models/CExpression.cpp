#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CExpression.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CExpression::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition()) +
            " -- content : \n";

    for (auto subContent : this->Composition())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return std::string(depth, ' ') + " - " + content;
}
