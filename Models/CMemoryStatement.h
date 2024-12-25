#ifndef CMEMORY_STATEMENT_H__
#define CMEMORY_STATEMENT_H__

#include <utility>
#include <vector>
#include <memory>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
	class CExpression;
	class CMemoryStatementAttribute;

	/// @brief Represents a single entry in the "MEMORIES" region
    class CMemoryStatement : public CParsedContentBase
    {   
    
        CRawEntry m_nameEntry;
        CRawEntry m_originHeaderEntry;
        std::shared_ptr<CMemoryStatementAttribute> m_attributes;
        CRawEntry m_colonEntry;
        CRawEntry m_lengthHeaderEntry;
        CRawEntry m_originAssignmentSymbolEntry;
        CRawEntry m_commaSeparatingOriginFromLengthEntry;
        CRawEntry m_lengthAssignmentSymbolEntry;
        std::shared_ptr<CExpression> m_originAssignment;
        std::shared_ptr<CExpression> m_lengthAssignment;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatement(const CRawEntry& nameEntry,
                                  const std::shared_ptr<CMemoryStatementAttribute>& attributes,
                                  const CRawEntry& colonEntry,
                                  const CRawEntry& originHeaderEntry,
                                  const CRawEntry& originAssignmentSymbolEntry,
                                  const CRawEntry& commaSeparatingOriginFromLengthEntry,
                                  const CRawEntry& lengthHeaderEntry,
                                  const CRawEntry& lengthAssignmentSymbolEntry,
                                  const std::shared_ptr<CExpression>& originRValue,
                                  const std::shared_ptr<CExpression>& lengthRValue,
                                  const std::vector<CRawEntry>& rawEntries,
                                  const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawEntries, violations),
              m_nameEntry(nameEntry),
			  m_originHeaderEntry(originHeaderEntry),
              m_attributes(attributes),
              m_colonEntry(colonEntry),
              m_lengthHeaderEntry(lengthHeaderEntry),
              m_originAssignmentSymbolEntry(originAssignmentSymbolEntry),
              m_commaSeparatingOriginFromLengthEntry(commaSeparatingOriginFromLengthEntry),
              m_lengthAssignmentSymbolEntry(lengthAssignmentSymbolEntry),
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

        /// @brief Reports back 'ORIGIN' header entry (the header itself)
        [[nodiscard]] CRawEntry OriginHeaderEntry() const
        {
            return m_originHeaderEntry;
        }

        /// @brief Reports back the comma separating the origin and the length statements
        [[nodiscard]] CRawEntry CommaSeparatingOriginFromLengthEntry() const
        {
            return m_originHeaderEntry;
        }

        /// @brief Reports back the Attributes of the statement (as CMemoryStatementAttribute)
        [[nodiscard]] std::shared_ptr<CMemoryStatementAttribute> Attributes() const
        {
            return m_attributes;
        }

        /// @brief Reports back 'Colon' entry
        [[nodiscard]] CRawEntry ColonEntry() const
        {
            return m_colonEntry;
        }

        /// @brief Reports back 'LENGTH' header entry
        [[nodiscard]] CRawEntry LengthHeaderEntry() const
        {
            return m_lengthHeaderEntry;
        }

        /// @brief Reports back the equal sign used in the length assignment
        [[nodiscard]] CRawEntry LengthAssignmentSymbolEntry() const
        {
            return m_lengthAssignmentSymbolEntry;
        }

        /// @brief Reports back 'ORIGIN' or 'org' assignment
        [[nodiscard]] std::shared_ptr<CExpression> OriginAssignment() const
        {
            return m_originAssignment;
        }

        /// @brief Reports back 'LENGTH' or 'l' or 'len' assignment
        [[nodiscard]] std::shared_ptr<CExpression> LengthAssignment() const
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
