#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <vector>
#include <string>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models 
{
    /// @brief Object representing a parsed Linker-Script
    class CLinkerScriptFile
    {

    private:
        std::vector<CLinkerScriptContentBase> m_content;
        std::string m_rawText;

    public:
        /// @brief Default constructor
        explicit CLinkerScriptFile(std::string rawText, std::vector<CLinkerScriptContentBase>&& content)
            : m_content(std::move(content)), m_rawText(rawText)
        {}

        /// @brief Default destructor
        ~CLinkerScriptFile()
        {}

    public:
        /// @brief Returns the content of the file
        const std::vector<CLinkerScriptEntryBase>& Content();

        /// @brief Returns the raw linker-script text
        const std::string& RawText();

        /// @brief Returns the full text the input component is composed of.
        /// @param entryToResolve Component to process
        /// @return The full text that constitutes the component.
        const std::string ResolveEntryText(CLinkerScriptEntryBase& entryToResolve);    
    };
}

#endif
