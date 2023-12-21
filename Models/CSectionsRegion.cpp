#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionsRegion.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CSectionsRegion::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @pos " + std::to_string(this->StartPosition()) + "\n";

    content += std::string(depth, ' ') + " -- entries: \n";
    for (auto subContent : this->Entries())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    return content;
}
