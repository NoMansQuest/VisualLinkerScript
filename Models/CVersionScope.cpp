#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CVersionScope.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CVersionScope::ToDebugInfo(uint32_t depth)
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition());

    content += std::string(depth, ' ') + " -- Parsed content: \n";
    for (auto subContent : this->ParsedContent())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    return std::string(depth, ' ') + " - " + content;
}
