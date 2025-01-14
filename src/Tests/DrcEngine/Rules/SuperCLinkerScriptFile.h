#pragma once

#include "Models/CLinkerScriptFile.h"

namespace VisualLinkerScript::Tests::DrcEngine::Rules
{
    using namespace VisualLinkerScript::Models::Raw;
    using namespace VisualLinkerScript::Models;

    /// @brief Object representing a parsed Linker-Script
    class SuperCLinkerScriptFile : public CLinkerScriptFile
    {
    public:
        /// @brief Default constructor
        SuperCLinkerScriptFile()
            : CLinkerScriptFile(
                "diagnostic_filename",
                "diagnostic_absolutePath",
                "diagnostic_contentOnDisk",
                false,
                "diagnostic_signatureOnDisk")
        {	        
        }

        /// @brief Resolve the raw-entry to what the test needs
        [[nodiscard]] std::string ResolveRawEntry(const CRawEntry& entryToResolve) const override
        {
            return entryToResolve.DiagnosticContent();
        }

        /// @brief Resolve content to what the test needs
        [[nodiscard]] std::string ResolveParsedContent(const CParsedContentBase& contentToResolve) const override
        {
            throw std::exception("Method not implemented.");
        }
    };
}
