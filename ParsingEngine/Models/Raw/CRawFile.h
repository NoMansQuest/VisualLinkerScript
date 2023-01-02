#ifndef CRAWFILE_H__
#define CRAWFILE_H__

#include <vector>
#include <memory>
#include <string>
#include "CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models::Raw 
{
    /// @brief This object represents a file processed by @see CPreliminaryParser
    class CRawFile
    {
    private:         
        std::vector<CRawEntry> m_content;
        std::string m_fileName;
        std::string m_fileContent;

    public:
        /// @brief Default constructor
        /// @param content Vector of CRawEntry, which the linker-script is composed of.
        explicit CRawFile(std::string&& fileContent,
                          const std::string fileName, 
                          std::vector<CRawEntry>&& content)
            : m_content(std::move(content)),
              m_fileName(fileName),
              m_fileContent(std::move(fileContent))
        {}

        /// @brief Default constructor
        /// @param content Vector of CRawEntry, which the linker-script is composed of.
        explicit CRawFile(const std::string fileContent,
                          const std::string fileName,
                          std::vector<CRawEntry>&& content)
            : m_content(std::move(content)),
              m_fileName(fileName),
              m_fileContent(fileContent)
        {}


    public:
        /// @brief Retried the content's vector        
        const std::vector<CRawEntry>& Content()
        {
            return this->m_content;
        }

        /// @brief Name of the loaded file        
        const std::string& FileName()
        {
            return this->m_fileName;
        }

        /// @brief Reports back the content of the linker-script file
        const std::string& FileContent()
        {
            return this->m_fileContent;
        }

        /// @brief Resolves the entry by returning the string-content the @see {entryToResolve} is
        ///        referring to.        
        const std::string& ResolveRawEntry(const CRawEntry& entryToResolve)
        {

        }

        
    };
}

#endif
