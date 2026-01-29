#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "Models/CMemoryStatement.h"
#include "Models/CMemoryStatementAttribute.h"
#include "Models/CExpression.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models;

const LinqVector<CViolationBase> CMemoryStatement::AggregateViolation() const
{
    LinqVector<CViolationBase> allViolations;
    FuseVectors(allViolations, this->Attributes()->AggregateViolation());
    FuseVectors(allViolations, this->OriginAssignment()->AggregateViolation());
    FuseVectors(allViolations, this->LengthAssignment()->AggregateViolation());
    FuseVectors(allViolations, this->Violations());
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
