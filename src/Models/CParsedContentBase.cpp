
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "Models/CLinkerScriptFile.h"
#include "Models/CParsedContentBase.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CParsedContentBase::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    const CParsedContentBase& thisReference = *this;
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CParsedContentBase") +
            " @pos " + std::to_string(this->StartPosition()) +
            " -- content : " + linkerScriptFile.ResolveParsedContent(thisReference);

    return std::string(depth, ' ') + " - " + content;
}
