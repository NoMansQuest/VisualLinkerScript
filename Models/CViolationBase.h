#ifndef CVIOLATIONBASE_H__
#define CVIOLATIONBASE_H__

#include <string>
#include <vector>
#include <memory>
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models 
{
	using namespace VisualLinkerScript::Models::Raw;

    /// @brief Type of violation.
    enum class EViolationType
    {
        LexerViolation,
        DrcViolation,
        ParserViolation
    };

    /// @brief This object contains base object for violations detected in the linkerscript
    class CViolationBase
    {
    public:
        virtual EViolationType Type() = 0;
    };
}

#endif // CVIOLATIONBASE_H__
