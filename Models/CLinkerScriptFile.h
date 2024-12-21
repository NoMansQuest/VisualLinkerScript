#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <sstream>
#include <vector>
#include <string>
#include "CViolationBase.h"
#include "CParsedContentBase.h"
#include "Raw/CRawFile.h"

namespace VisualLinkerScript::ParsingEngine
{
	class CParserViolation;
}

namespace VisualLinkerScript::Models 
{
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Object representing a parsed Linker-Script
    class CLinkerScriptFile
    {
    private:
        std::string m_fileName;
        std::string m_absolutePath;
        std::string m_contentOnDisk;
        std::string m_content;
        std::string m_md5Hash;
        std::vector<std::string> m_lines;
        bool m_isOnDisk;

        LinqVector<CViolationBase> m_drcViolations;
        LinqVector<CViolationBase> m_parserViolations;
        LinqVector<CViolationBase> m_lexerViolations;

        std::unordered_map<uint32_t, CIndentationInfo> m_indentationData;
        std::vector<CRawEntry> m_lexedContent;
        LinqVector<CParsedContentBase> m_parsedContent;
        std::shared_ptr<CRawFile> m_rawFile;


        /// @brief Breaks the given text into lines and fills in the lines vector
        void UpdateLines(const std::string& content)
        {
            this->m_lines.clear();
            std::istringstream stream(content);
            std::string line;

            while (std::getline(stream, line)) 
            {
                line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                this->m_lines.emplace_back(line);
            }
        }

    public:
        /// @brief Default constructor
        explicit CLinkerScriptFile(
			const std::string& fileName, 
            const std::string& absolutePath,
            const std::string& contentOnDisk,
            const bool isOnDisk = false,
            const std::string& signatureOnDisk = "")
        {
            this->m_contentOnDisk = contentOnDisk;
            this->m_fileName = fileName;
            this->m_absolutePath = absolutePath;
            this->m_isOnDisk = isOnDisk;
            this->m_md5Hash = signatureOnDisk;
            this->m_content = contentOnDisk;
        }

        /// @brief Returns the content of the file.
        [[nodiscard]] const std::string& ContentOnDisk() const
        {
            return this->m_contentOnDisk;
        }

        /// @brief Returns the content of the file.
        [[nodiscard]] const std::string& Content() const
        {
            return this->m_content;
        }

        /// @brief Returns the line content.
        [[nodiscard]] const std::vector<std::string>& Lines() const
        {
            return this->m_lines;
        }

        /// @brief Returns the content of the file.
        void UpdateContent(const std::string& newContent)
        {
            this->m_content = newContent;
            this->UpdateLines(newContent);
        }

        /// @brief Retried the lexed content
        [[nodiscard]] const std::vector<CRawEntry>& LexedContent() const {
            return this->m_lexedContent;
        }

        /// @brief Returns the content of the file
        [[nodiscard]] const LinqVector<CParsedContentBase>& ParsedContent() const {
            return this->m_parsedContent;
        }

        /// @brief Reports back lexer violations detected at root level of the linker-script file
        [[nodiscard]] const LinqVector<CViolationBase>& LexerViolations() const {
            return this->m_lexerViolations;
        }

        /// @brief Reports back parser violations detected at root level of the linker-script file
        [[nodiscard]] const LinqVector<CViolationBase>& ParserViolations() const {
            return this->m_parserViolations;
        }

        /// @brief Reports back DRC violations detected at root level of the linker-script file
        [[nodiscard]] const LinqVector<CViolationBase>& DrcViolations() const {
            return this->m_drcViolations;
        }

        /// @brief Returns the name of the file without the path
        [[nodiscard]] std::string FileName() const {
            return this->m_fileName;
        }

        /// @brief Returns the absolute file path
        [[nodiscard]] std::string AbsoluteFilePath() const {
            return this->m_absolutePath;
        }

        /// @brief Returns the MD5 hash of the file.
        [[nodiscard]] std::string MD5Hash() const {
            return this->m_md5Hash;
        }

        /// @brief Returns indentation data generated by the lexer.
        [[nodiscard]] std::unordered_map<uint32_t, CIndentationInfo> IndentationData() const {
            return this->m_indentationData;
        }

        /// @brief Resolves the entry by returning the string-content the @see {entryToResolve} is
        ///        referring to.        
        [[nodiscard]] std::string ResolveRawEntry(const CRawEntry& entryToResolve) const
        {
            return this->Content().substr(entryToResolve.StartPosition(), entryToResolve.Length());
        }

        /// @brief Returns the full text the input component is composed of.
        /// @param contentToResolve Component to process
        /// @return The full text that constitutes the component.
        [[nodiscard]] std::string ResolveParsedContent(const CParsedContentBase& contentToResolve) const
        {
            const auto startPosition = contentToResolve.RawEntries().front().StartPosition();
            const auto endPosition = contentToResolve.RawEntries().back().StartPosition() +contentToResolve.RawEntries().back().Length() - 1;
            return this->Content().substr(startPosition, endPosition - startPosition + 1);
        }

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] virtual std::string ToDebugInfo(uint32_t depth) const;

        /// @brief Update parsed content
        void UpdateParsedContent(const LinqVector<CParsedContentBase>& parsedContent, 
								 const LinqVector<CViolationBase>& parserViolations) {
            this->m_parsedContent = parsedContent;
            this->m_parserViolations = parserViolations;
        }

        /// @brief Updates DRC violations associated with this linker-script file
        void UpdateDrcViolations(LinqVector<CViolationBase> drcViolations) {
            this->m_drcViolations = std::move(drcViolations);
        }

        /// @brief Updates lexer violations associated with this linker-script file
        void UpdateLexerData(
				const std::vector<CRawEntry>& lexerContent,
				const std::unordered_map<uint32_t, CIndentationInfo>& indentationData,
				const LinqVector<CViolationBase>& lexerViolations)
    	{
            this->m_lexedContent = lexerContent;
            this->m_indentationData = indentationData;
            this->m_lexerViolations = lexerViolations;
        }

        /// @brief Update the content of the file.
        void UpdateFileContent(
				const std::string& fileContent,
				const std::string& md5Hash)
        {
            this->m_contentOnDisk = fileContent;
            this->m_md5Hash = md5Hash;
            this->UpdateLines(fileContent);
        }

        /// @brief Update the data on of the Linker-Script file.
        /// @brief This update is only made when the file is actually saved on the disk.
        void UpdateFileName(
				const std::string& fileName,
				const std::string& absoluteFilePath)
        {
            this->m_fileName = fileName;
            this->m_absolutePath = absoluteFilePath;
            this->m_isOnDisk = true;
        }
    };
}

#endif