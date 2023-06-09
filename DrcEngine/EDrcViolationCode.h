#ifndef EPARSERVIOLATIONCODE_H__
#define EPARSERVIOLATIONCODE_H__

#include <cstdint>

namespace VisualLinkerScript::DrcEngine
{
    /// @brief An exhaustive list of all known violations.
    enum class EDrcViolationCode : uint32_t
    {
        WasExpectingProgramHeaderTypeHere,
    }
}