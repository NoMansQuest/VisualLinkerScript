#ifndef CINCLUDE_COMMAND_H__
#define CINCLUDE_COMMAND_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "Include" command, found at linker script file root
    class CIncludeCommand : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_includeCommandHeaderEntry;
        CRawEntry m_includeFileEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CIncludeCommand(CRawEntry includeCommandHeaderEntry,
                                 CRawEntry includeFileEntry,
                                 std::vector<CRawEntry>&& rawElements,
                                 std::vector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_includeCommandHeaderEntry(includeCommandHeaderEntry),
              m_includeFileEntry(includeFileEntry)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override {
            return ContentType::ProcedureCall;
        }

        /// @brief Reports the entry contianing the "INCLUDE" part
        const CRawEntry& IncludeCommandHeaderEntry() {
            return this->m_includeCommandHeaderEntry;
        }

        /// @brief Reports the entry contianing the "file" part
        const CRawEntry& IncludeFileEntry() {
            return this->m_includeFileEntry;
        }
    };
}

#endif
