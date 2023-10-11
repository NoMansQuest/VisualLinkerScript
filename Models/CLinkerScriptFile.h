#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <vector>
#include <string>
#include "CViolationBase.h"
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawFile.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models 
{
    /// @brief Object representing a parsed Linker-Script
    class CLinkerScriptFile
    {
    private:
        SharedPtrVector<CViolationBase> m_violations;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_content;        
        std::shared_ptr<CRawFile> m_rawFile;

    public:
        /// @brief Default constructor
        explicit CLinkerScriptFile(std::shared_ptr<CRawFile> rawFile,
                                   SharedPtrVector<CLinkerScriptContentBase>&& content,
                                   SharedPtrVector<CViolationBase>&& violations)
            : m_violations(std::move(violations)),
              m_content(std::move(content)),              
              m_rawFile(rawFile)
        {}

    public:
        /// @brief Returns the content of the file
        const SharedPtrVector<CLinkerScriptContentBase>& Content() {
            return this->m_content;
        }

        /// @brief Reports back violations detected at root level of the linker-script file
        const SharedPtrVector<CViolationBase>& Violations() {
            return this->m_violations;
        }

        /// @brief Returns the raw linker-script file
        std::shared_ptr<CRawFile> RawFile() {
            return this->m_rawFile;
        }

        /// @brief Returns the name of he file without the path
        std::string FileName() {
            return this->m_rawFile->FileName();
        }

        /// @brief Returns the absolute file path
        std::string AbsoluteFilePath() {
            return this->m_rawFile->AbsoluteFilePath();
        }

        /// @brief Returns the full text the input component is composed of.
        /// @param entryToResolve Component to process
        /// @return The full text that constitutes the component.
        std::string ResolveEntryText(CLinkerScriptContentBase& contentToResolve);

        /// @brief Returns the full text the input component is composed of.
        /// @param entryToResolve Component to process
        /// @return The full text that constitutes the component.
        std::string ResolveEntryText(const CRawEntry& rawEntryToResolve);

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth);
    };
}

#endif
