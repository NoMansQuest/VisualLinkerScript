#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "CMemoryStatement.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const SharedPtrVector<CViolationBase> CMemoryStatement::AggregateViolation() const
{
    SharedPtrVector<CViolationBase> allViolations;
    allViolations.insert(allViolations.end(), this->Attributes()->AggregateViolation().cbegin(), this->Attributes()->AggregateViolation().cend());
    allViolations.insert(allViolations.end(), this->OriginAssignment()->AggregateViolation().cbegin(), this->OriginAssignment()->AggregateViolation().cend());
    allViolations.insert(allViolations.end(), this->LengthAssignment()->AggregateViolation().cbegin(), this->LengthAssignment()->AggregateViolation().cend());
    allViolations.insert(allViolations.end(), this->Violations().begin(), this->Violations().end());
    return allViolations; // Note: R-Value optimization ensures this vector isn't unnecessarily copied.
}

/// @brief Produces debug information on what this object represents.
const std::string CMemoryStatement::ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const
{
    std::string content =
            std::string(depth, ' ') + " - " +
            (this->Violations().size() > 0 ? "[ ** ] " : "[ OK ] ") +
            std::string("CMemoryStatement") +
            " @pos " + std::to_string(this->StartPosition()) + "\n";

    content += std::string(depth, ' ') +  " -- Attributes :" + this->Attributes()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";;
    content += std::string(depth, ' ') +  " -- LengthAssignments : \n" + this->LengthAssignment()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";
    content += std::string(depth, ' ') +  " -- OriginAssignments : \n" + this->OriginAssignment()->ToDebugInfo(depth + 4, linkerScriptFile) + "\n";

    return content;
}