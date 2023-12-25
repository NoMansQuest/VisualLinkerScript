#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionOutputOverlay.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CSectionOutputOverlay::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CSectionOutputOverlay") +
            " @pos " + std::to_string(this->StartPosition());

    content += std::string(depth, ' ') + " -- Pre-colon content : \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Post-colon content : \n";

    for (auto subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + "ToVmaRegion: " + this->ToVmaRegion()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    content += std::string(depth, ' ') + "AtLmaRegionFunction: " + this->AtLmaRegionFunction()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    content += std::string(depth, ' ') + "AtLmaRegionFunction: " + this->ToVmaRegion()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    content += std::string(depth, ' ') + " -- Inner content : \n";

    for (auto subContent : this->InnerContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Program-headers content : \n";

    for (auto subContent : this->ProgramHeaders())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + "AtLmaRegionFunction: " + this->FillExpression()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    return content;
}
