#ifndef CMEMORY_STATEMENT_H__
#define CMEMORY_STATEMENT_H__

#include <vector>
#include <memory>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CMemoryStatement : public CParsedContentBase
    {   
    private:
        CRawEntry m_nameEntry;
        std::shared_ptr<CParsedContentBase> m_attributes;
        CRawEntry m_colonEntry;
        std::shared_ptr<CParsedContentBase> m_originAssignment;
        std::shared_ptr<CParsedContentBase> m_lengthAssignment;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatement(CRawEntry nameEntry,
                                  std::shared_ptr<CParsedContentBase> attributes,
                                  CRawEntry colonEntry,
                                  CRawEntry originHeaderEntry,
                                  CRawEntry originAssignmentSymbolEntry,
                                  CRawEntry commaSeparatingOriginFromLengthEntry,
                                  CRawEntry lengthHeaderEntry,
                                  CRawEntry lengthAssignmentSymbolEntry,
                                  std::shared_ptr<CParsedContentBase> originRValue,
                                  std::shared_ptr<CParsedContentBase> lengthRValue,
                                  std::vector<CRawEntry>&& rawEntries,
                                  LinqVector<CViolationBase>&& violations)
            : CParsedContentBase(std::move(rawEntries), std::move(violations)),
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
        [[nodiscard]] CRawEntry NameEntry() const
        {
            return m_nameEntry;
        }

        /// @brief Reports back the Attributes of the statement (as CMemoryStatementAttribute)
        [[nodiscard]] std::shared_ptr<CParsedContentBase> Attributes() const
        {
            return m_attributes;
        }

        /// @brief Reports back 'Colon' entry
        [[nodiscard]] CRawEntry ColonEntry() const
        {
            return m_colonEntry;
        }

        /// @brief Reports back 'ORIGIN' or 'org' assignment
        [[nodiscard]] std::shared_ptr<CParsedContentBase> OriginAssignment() const
        {
            return m_originAssignment;
        }

        /// @brief Reports back 'LENGTH' or 'l' or 'len' assignment
        [[nodiscard]] std::shared_ptr<CParsedContentBase> LengthAssignment() const
        {
            return m_lengthAssignment;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] virtual const LinqVector<CViolationBase> AggregateViolation() const;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };    
}

#endif
