#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include "CMemoryStatement.h"

using namespace VisualLinkerScript::Models;

/// @brief Produces debug information on what this object represents.
const std::string CMemoryStatement::ToDebugInfo(uint32_t depth)
{
    std::string content =
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string(typeid(*this).name()) +
            " @line " + std::to_string(this->StartPosition());

    content += std::string(depth, ' ') +  " -- Attributes :" + this->Attributes()->ToDebugInfo(depth + 4) + "\n";;
    content += std::string(depth, ' ') +  " -- LengthAssignments : \n" + this->LengthAssignment()->ToDebugInfo(depth + 4) + "\n";
    content += std::string(depth, ' ') +  " -- OriginAssignments : \n" + this->OriginAssignment()->ToDebugInfo(depth + 4) + "\n";

    return std::string(depth, ' ') + " - " + content;
}
