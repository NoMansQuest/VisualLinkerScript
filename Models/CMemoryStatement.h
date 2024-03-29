#ifndef CMEMORY_STATEMENT_H__
#define CMEMORY_STATEMENT_H__

#include <vector>
#include <memory>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CMemoryStatement : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_nameEntry;
        std::shared_ptr<CLinkerScriptContentBase> m_attributes;
        CRawEntry m_colonEntry;
        std::shared_ptr<CLinkerScriptContentBase> m_originAssignment;
        std::shared_ptr<CLinkerScriptContentBase> m_lengthAssignment;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatement(CRawEntry nameEntry,
                                  std::shared_ptr<CLinkerScriptContentBase> attributes,
                                  CRawEntry colonEntry,
                                  CRawEntry originHeaderEntry,
                                  CRawEntry originAssignmentSymbolEntry,
                                  CRawEntry commaSeparatingOriginFromLengthEntry,
                                  CRawEntry lengthHeaderEntry,
                                  CRawEntry lengthAssignmentSymbolEntry,
                                  std::shared_ptr<CLinkerScriptContentBase> originRValue,
                                  std::shared_ptr<CLinkerScriptContentBase> lengthRValue,
                                  std::vector<CRawEntry>&& rawEntries,
                                  SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawEntries), std::move(violations)),
              m_nameEntry(nameEntry),
              m_attributes(attributes),
              m_colonEntry(colonEntry),
              m_originAssignment(originRValue),
              m_lengthAssignment(lengthRValue)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryStatement;
        }

        /// @brief Reports back 'Name' entry
        const CRawEntry NameEntry() const
        {
            return m_nameEntry;
        }

        /// @brief Rpeorts back the Attributes of the statement (as CMemoryStatementAttribute)
        const std::shared_ptr<CLinkerScriptContentBase> Attributes() const
        {
            return m_attributes;
        }

        /// @brief Reports back 'Colon' entry
        const CRawEntry ColonEntry() const
        {
            return m_colonEntry;
        }

        /// @brief Rpeorts back 'ORIGIN' or 'org' assignment
        const std::shared_ptr<CLinkerScriptContentBase> OriginAssignment() const
        {
            return m_originAssignment;
        }

        /// @brief Reports back 'LENGTH' or 'l' or 'len' assignment
        std::shared_ptr<CLinkerScriptContentBase> LengthAssignment() const
        {
            return m_lengthAssignment;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };    
}

#endif
