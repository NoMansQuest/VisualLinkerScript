#ifndef EDRCVIOLATIONCODE_H__
#define EDRCVIOLATIONCODE_H__

#include <string>
#include <cstdint>

namespace VisualLinkerScript::DrcEngine
{
    /// @brief An exhaustive list of all known DRC related violations 
    enum class EDrcViolationCode: uint32_t
    {
        WasExpectingProgramHeaderTypeHere,
        EntryDirectiveDefinedMoreThanOnce,
        IncludedFilesArePresentRule,
        RecursiveIncludeChainDetected,
        DuplicateNameForMemoryStatement,
        SectionsRegionDefinedMultipleTimes,
        MemoryRegionDefinedMultipleTimes,
    };

    /// @brief Maps DRC violation code to string
    std::string MapDrcViolationToDescription(EDrcViolationCode code);

    /// @brief Maps DRC violation to code.
    std::string MapDrcViolationToCode(EDrcViolationCode code);
}

#endif // EDRCVIOLATIONCODE_H__
