#ifndef CPHDRS_REGION_H__
#define CPHDRS_REGION_H__

#include <vector>
#include "Models/CParsedContentBase.h"
#include "Models/CPhdrsStatement.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents the PHDRS region in the Linker-Script.
    class CPhdrsRegion : public CParsedContentBase
    {  
    private:
        std::vector<std::shared_ptr<CParsedContentBase>> m_statements;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_phdrsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CPhdrsRegion(const CRawEntry& phdrsHeaderEntry,
                              const CRawEntry& openingBracketEntry,
                              const CRawEntry& closingBracketEntry,
                              const std::vector<std::shared_ptr<CParsedContentBase>>& phdrsStatements,
                              const std::vector<CRawEntry>& rawElements,
                              const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_statements(phdrsStatements),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_phdrsHeaderEntry(phdrsHeaderEntry)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProgramHeaderRegion;
        }

        /// @brief Reports back PHDR statements
        [[nodiscard]] const std::vector<std::shared_ptr<CParsedContentBase>>& Statements() const
        {
            return m_statements;
        }

        /// @brief Reports back the entry containing the 'PHDRS' header  
        [[nodiscard]] CRawEntry PhdrsHeaderEntry() const
        {
            return this->m_phdrsHeaderEntry;
        }

        /// @brief Reports back the entry containing the "{" symbol
        [[nodiscard]] CRawEntry OpeningBracketEntry() const
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the entry containing the "}" symbol
        [[nodiscard]] CRawEntry ClosingBracketEntry() const
        {
            return this->m_closingBracketEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] virtual const LinqVector<CViolationBase> AggregateViolation() const;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
