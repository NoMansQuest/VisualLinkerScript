#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <vector>
#include <string>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawFile.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models 
{
    /// @brief Object representing a parsed Linker-Script
    class CLinkerScriptFile
    {

    private:
        std::vector<CLinkerScriptContentBase> m_content;
        CRawFile m_rawFile;

    public:
        /// @brief Default constructor
        explicit CLinkerScriptFile(CRawFile&& rawFile, std::vector<CLinkerScriptContentBase>&& content)
            : m_content(std::move(content)), 
              m_rawFile(std::move(rawFile))
        {}

        /// @brief Default destructor
        ~CLinkerScriptFile()
        {}

    public:
        /// @brief Returns the content of the file
        const std::vector<CLinkerScriptContentBase>& Content()
        {
            return this->m_content;
        }

        /// @brief Returns the raw linker-script file
        const CRawFile& RawFile()
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

            return this->m_rawFile.FileContent().substr(startPosition, endPosition - startPosition + 1);
        }
    };
}

#endif
