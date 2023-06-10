#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <vector>
#include <string>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawFile.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models 
{
    /// @brief Object representing a parsed Linker-Script
    class CLinkerScriptFile
    {
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_content;
        std::vector<CViolationBase> m_violations;
        std::shared_ptr<CRawFile> m_rawFile;

    public:
        /// @brief Default constructor
        explicit CLinkerScriptFile(std::shared_ptr<CRawFile> rawFile,
                                   std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& content,
                                   std::vector<CViolationBase>&& violations)
            : m_content(std::move(content)), 
              m_violations(std::move(violations)),
              m_rawFile(rawFile)
        {}

    public:
        /// @brief Returns the content of the file
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Content()
        {
            return this->m_content;
        }

        /// @brief Reports back violations detected at root level of the linker-script file
        const std::vector<CViolationBase>& Violations()
        {
            return this->m_violations;
        }

        /// @brief Returns the raw linker-script file
        const std::shared_ptr<CRawFile> RawFile()
        {
            return this->m_rawFile;
        }

        /// @brief Returns the full text the input component is composed of.
        /// @param entryToResolve Component to process
        /// @return The full text that constitutes the component.
        const std::string ResolveEntryText(CLinkerScriptContentBase& contentToResolve)
        {
            auto startPosition = contentToResolve.RawEntries().front().StartPosition();
            auto endPosition = contentToResolve.RawEntries().back().StartPosition() +
                               contentToResolve.RawEntries().back().Length() - 1;

            return this->m_rawFile->FileContent().substr(startPosition, endPosition - startPosition + 1);
        }
    };
}

#endif
