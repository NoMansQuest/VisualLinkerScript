#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CMemoryStatement.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CMemoryStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @pos " + std::to_string(this->StartPosition()) + "\n";

    content += std::string(depth, ' ') +  " -- Attributes :" + this->Attributes()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";;
    content += std::string(depth, ' ') +  " -- LengthAssignments : \n" + this->LengthAssignment()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    content += std::string(depth, ' ') +  " -- OriginAssignments : \n" + this->OriginAssignment()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";

    return content;
}
