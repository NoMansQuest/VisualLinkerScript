#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CMemoryRegion.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CMemoryRegion::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CMemoryRegion") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content : \n";

    for (auto subContent : this->Statements())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return std::string(depth, ' ') + " - " + content;
}
