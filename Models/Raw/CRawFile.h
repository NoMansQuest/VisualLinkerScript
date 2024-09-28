#ifndef CRAWFILE_H__
#define CRAWFILE_H__

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "CRawEntry.h"
#include "Models/CIndentationInfo.h"

namespace VisualLinkerScript::Models::Raw 
{
    /// @brief This object represents a file processed by @see CLinkerScriptLexer
    class CRawFile
    {
        std::vector<CRawEntry> m_content;
        std::unordered_map<uint32_t, CIndentationInfo> m_indentationData;
        std::string m_fileName;
        std::string m_absoluteFilePath;
        std::string m_fileContent;

    public:
        /// @brief Default constructor
        /// @param fileContent Content of the linker script file.
        /// @param fileName Name of the linker script file
        /// @param absoluteFilePath Absolute file path
        /// @param indentationData Indentation data, per line of code
        /// @param content Vector of CRawEntry, which the linker-script is composed of.
        explicit CRawFile(const std::string& fileContent,
                          const std::string& fileName,
                          const std::string& absoluteFilePath,
						  const std::unordered_map<uint32_t, CIndentationInfo>& indentationData,
                          const std::vector<CRawEntry>& content) :
              m_content(content),
              m_indentationData(indentationData),
              m_fileName(fileName),
			  m_absoluteFilePath(absoluteFilePath),
              m_fileContent(fileContent)
        {}

    public:
        /// @brief Retried the content's vector        
        const std::vector<CRawEntry>& Content(){
            return this->m_content;
        }

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

        /// @brief Returns indentation data generated by the lexer.
        std::unordered_map<uint32_t, CIndentationInfo> IndentationData() const {
            return this->m_indentationData;
        }

        /// @brief Resolves the entry by returning the string-content the @see {entryToResolve} is
        ///        referring to.        
        const std::string ResolveRawEntry(const CRawEntry& entryToResolve) const {
            return this->m_fileContent.substr(entryToResolve.StartPosition(),entryToResolve.Length());
        }        
    };
}

#endif
