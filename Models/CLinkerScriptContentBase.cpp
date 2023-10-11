
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CLinkerScriptFile.h"
#include "CLinkerScriptContentBase.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CLinkerScriptContentBase::ToDebugInfo(uint32_t depth)
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition()) +
            " -- content : " + this->ParentLinkerScriptFile()->ResolveEntryText(*this);

    return std::string(depth, ' ') + " - " + content;
}
