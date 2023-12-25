#ifndef EDRCVIOLATIONCODE_H__
#define EDRCVIOLATIONCODE_H__

#include <cstdint>

namespace VisualLinkerScript::DrcEngine
{
    /// @brief An exhaustive list of all known DRC related violations 
    enum class EDrcViolationCode : uint32_t
    {
        WasExpectingProgramHeaderTypeHere,
        EntryDirectiveDefinedMoreThanOnce,
        IncludedFilesArePresentRule,
        RecursiveIncludeChainDetected,
        DuplicateNameForMemoryStatement,
        SectionsRegionDefinedMultipleTimes,
        MemoryRegionDefinedMultipleTimes,
    };
}

#endif // EDRCVIOLATIONCODE_H__
