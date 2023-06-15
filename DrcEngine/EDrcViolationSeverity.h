#ifndef EDRCVIOLATIONSEVERITY_H__
#define EDRCVIOLATIONSEVERITY_H__

#include <cstdint>

namespace VisualLinkerScript::DrcEngine
{
    enum class EDrcViolationSeverity : uint32_t
    {
        Information,
        Warning,
        Error
    };
}

#endif // EDRCVIOLATIONSEVERITY_H
