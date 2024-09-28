#ifndef CPHDRS_REGION_H__
#define CPHDRS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CPhdrsStatement.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents the PHDRS region in the Linker-Script.
    class CPhdrsRegion : public CLinkerScriptContentBase
    {  
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_statements;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_phdrsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CPhdrsRegion(const CRawEntry& phdrsHeaderEntry,
                              const CRawEntry& openingBracketEntry,
                              const CRawEntry& closingBracketEntry,
                              const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& phdrsStatements,
                              const std::vector<CRawEntry>& rawElements,
                              const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_statements(phdrsStatements),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_phdrsHeaderEntry(phdrsHeaderEntry)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::PhdrsRegion;
        }

        /// @brief Reports back PHDR statements
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Statements() const
        {
            return m_statements;
        }

        /// @brief Reports back the entry containing the 'PHDRS' header  
        const CRawEntry PhdrsHeaderEntry() const
        {
            return this->m_phdrsHeaderEntry;
        }

        /// @brief Reports back the entry containing the "{" symbol
        const CRawEntry OpeningBracketEntry() const
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the entry containing the "}" symbol
        const CRawEntry ClosingBracketEntry() const
        {
            return this->m_closingBracketEntry;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
