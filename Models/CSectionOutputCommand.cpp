#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionOutputCommand.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CSectionOutputCommand::ToDebugInfo(uint32_t depth)
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition());

    content += std::string(depth, ' ') + " -- Pre-colon content : \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    content += std::string(depth, ' ') + " -- Post-colon content : \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    content += std::string(depth, ' ') + "ToVmaRegion: " + this->ToVmaRegion()->ToDebugInfo(depth + 4) + "\n";
    content += std::string(depth, ' ') + "AtLmaRegionFunction: " + this->AtLmaRegionFunction()->ToDebugInfo(depth + 4) + "\n";
    content += std::string(depth, ' ') + "AtLmaRegionFunction: " + this->ToVmaRegion()->ToDebugInfo(depth + 4) + "\n";
    content += std::string(depth, ' ') + " -- Inner content : \n";

    for (auto subContent : this->InnerContent())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    content += std::string(depth, ' ') + " -- Program-headers content : \n";

    for (auto subContent : this->ProgramHeaders())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    content += std::string(depth, ' ') + "AtLmaRegionFunction: " + this->FillExpression()->ToDebugInfo(depth + 4) + "\n";
    return std::string(depth, ' ') + " - " + content;
}
