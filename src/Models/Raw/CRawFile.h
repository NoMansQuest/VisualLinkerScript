#ifndef CRAWFILE_H__
#define CRAWFILE_H__

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "Models/Raw/CRawEntry.h"
#include "Models/CIndentationInfo.h"

namespace VisualLinkerScript::Models::Raw 
{
    /// @brief This object represents a file processed by @see CLinkerScriptLexer
    class CRawFile
    {        
        
        std::string m_fileName;
        std::string m_absoluteFilePath;
        std::string m_fileContent;

    public:
        /// @brief Default constructor
        /// @param fileContent Content of the linker script file.
        /// @param fileName Name of the linker script file
        /// @param absoluteFilePath Absolute file path
        explicit CRawFile(const std::string& fileContent,
                          const std::string& fileName,
                          const std::string& absoluteFilePath) :
              m_fileName(fileName),
			  m_absoluteFilePath(absoluteFilePath),
              m_fileContent(fileContent)
        {}

    public:


        /// @brief Name of the loaded file (full-path)
        std::string FileName() const {
            return this->m_fileName;
        }

        /// @brief Reports back the content of the linker-script file
        std::string FileContent() const {
            return this->m_fileContent;
        }

        /// @brief Returns the absolute file path (parent folder + file name).
        std::string AbsoluteFilePath() const {
            return this->m_absoluteFilePath;
        }

   
    };
}

#endif
