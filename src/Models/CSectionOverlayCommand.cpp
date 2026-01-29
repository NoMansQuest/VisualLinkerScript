#include <memory>
#include <string>
#include <stdint.h>

#include "Models/CSectionOverlayCommand.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const LinqVector<CViolationBase> CSectionOverlayCommand::AggregateViolation() const
{
    LinqVector<CViolationBase> allViolations;

    for (const auto& childEntry : this->PreColonContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }

    for (const auto& childEntry : this->PostColonContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }

    for (const auto& childEntry : this->InnerContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }

    for (const auto& childEntry : this->EndingContent())
    {
        FuseVectors(allViolations, childEntry->AggregateViolation());
    }

    FuseVectors(allViolations, this->Violations());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CSectionOverlayCommand::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CSectionOverlayCommand") +
            " @pos " + std::to_string(this->StartPosition()) + "\n";

    content += std::string(depth, ' ') + " -- PreColon content: \n";

    for (const auto subContent : this->PreColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- PostColon content: \n";

    for (const auto& subContent : this->PostColonContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Inner content: \n";

    for (const auto& subContent : this->InnerContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    }

    content += std::string(depth, ' ') + " -- Ending content: \n";

    for (const auto& subContent : this->EndingContent())
    {
        content += subContent->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";

    }

    return content;
}
