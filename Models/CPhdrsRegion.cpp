#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CPhdrsRegion.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CPhdrsRegion::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content : \n";

    for (auto subContent : this->Statements())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return content;
}
