#include <memory>
#include <string>
#include <stdint.h>

#include "CSectionOverlayCommand.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CSectionOverlayCommand::ToDebugInfo(uint32_t depth)
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition());

    content += std::string(depth, ' ') + " -- Start-address: " + this->StartAddressExpression()->ToDebugInfo(depth + 4) + "\n";
    content += std::string(depth, ' ') + " -- Inner content: \n";

    for (auto subContent : this->ParsedContent())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    content +=  std::string(depth, ' ') + " -- Fill expression : " +  this->FillExpression()->ToDebugInfo(depth + 4) + "\n";
    content += std::string(depth, ' ') + " -- Program headers : \n";

    for (auto subContent : this->ProgramHeaders())
    {
        content += subContent->ToDebugInfo(depth + 4) + "\n";
    }

    return std::string(depth, ' ') + " - " + content;
}
