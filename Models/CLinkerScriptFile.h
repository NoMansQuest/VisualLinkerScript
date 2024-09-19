#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <vector>
#include <string>
#include "CViolationBase.h"
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawFile.h"

namespace VisualLinkerScript::Models 
{
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Object representing a parsed Linker-Script
    class CLinkerScriptFile
    {
    private:
        SharedPtrVector<CViolationBase> m_drcViolations;
        SharedPtrVector<CViolationBase> m_parserViolations;
        SharedPtrVector<CViolationBase> m_lexerViolations;
        SharedPtrVector<CLinkerScriptContentBase> m_parsedContent;
        std::shared_ptr<CRawFile> m_rawFile;

    public:
        /// @brief Default constructor
        explicit CLinkerScriptFile(std::shared_ptr<CRawFile> rawFile)
            : m_rawFile(std::move(rawFile))
        {}

        /// @brief Returns the content of the file
        [[nodiscard]] const SharedPtrVector<CLinkerScriptContentBase>& ParsedContent() const {
            return this->m_parsedContent;
        }

        /// @brief Reports back lexer violations detected at root level of the linker-script file
        [[nodiscard]] const SharedPtrVector<CViolationBase>& LexerViolations() const {
            return this->m_lexerViolations;
        }

        /// @brief Reports back parser violations detected at root level of the linker-script file
        [[nodiscard]] const SharedPtrVector<CViolationBase>& ParserViolations() const {
            return this->m_parserViolations;
        }

        /// @brief Reports back DRC violations detected at root level of the linker-script file
        [[nodiscard]] const SharedPtrVector<CViolationBase>& DrcViolations() const {
            return this->m_drcViolations;
        }

        /// @brief Returns the raw linker-script file
        [[nodiscard]] std::shared_ptr<CRawFile> RawFile() const {
            return this->m_rawFile;
        }

        /// @brief Returns the name of the file without the path
        [[nodiscard]] std::string FileName() const {
            return this->m_rawFile->FileName();
        }

        /// @brief Returns the absolute file path
        [[nodiscard]] std::string AbsoluteFilePath() const {
            return this->m_rawFile->AbsoluteFilePath();
        }

        /// @brief Returns the full text the input component is composed of.
        /// @param contentToResolve Component to process
        /// @return The full text that constitutes the component.
        [[nodiscard]] std::string ResolveEntryText(const CLinkerScriptContentBase& contentToResolve) const;

        /// @brief Returns the full text the input component is composed of.
        /// @param rawEntryToResolve Component to process
        /// @return The full text that constitutes the component.
        [[nodiscard]] std::string ResolveEntryText(const CRawEntry& rawEntryToResolve) const;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const virtual std::string ToDebugInfo(uint32_t depth) const;

        /// @brief Update parsed content
        void UpdateParsedContent(SharedPtrVector<CLinkerScriptContentBase> parsedContent)
        {
            this->m_parsedContent = std::move(parsedContent);
        }

        /// @brief Updates parser violations associated with this linker-script file
        void UpdateParserViolations(SharedPtrVector<CViolationBase> parserViolations)
        {
            this->m_parserViolations = std::move(parserViolations);
        }

        /// @brief Updates DRC violations associated with this linker-script file
        void UpdateDrcViolations(SharedPtrVector<CViolationBase> drcViolations)
        {
            this->m_drcViolations = std::move(drcViolations);
        }

        /// @brief Updates lexer violations associated with this linker-script file
        void UpdateLexerViolations(SharedPtrVector<CViolationBase> lexerViolations)
        {
            this->m_lexerViolations = std::move(lexerViolations);
        }
    };
}

#endif
