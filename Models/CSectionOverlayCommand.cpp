#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionOverlayCommand.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CSectionOverlayCommand::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition());

    content += std::string(depth, ' ') + " -- PreColon content: \n";

    for (auto subContent : this->PreColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- PostColon content: \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Inner content: \n";

    for (auto subContent : this->InnerContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Ending content: \n";

    for (auto subContent : this->EndingContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";

    }

    return std::string(depth, ' ') + " - " + content;
}
