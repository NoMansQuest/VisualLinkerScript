#ifndef CPHDRS_REGION_H__
#define CPHDRS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CPhdrsStatement.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the PHDRS region in the Linker-Script.
    class CPhdrsRegion : public CLinkerScriptContentBase
    {  
    private:
        std::vector<CPhdrsStatement> m_phdrsStatements;
        CRawEntry m_openningBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_phdrsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of
        /// @param phdrsStatements A list of PHDR statements
        /// @param violations Violations found in the current element
        explicit CPhdrsRegion(CRawEntry phdrsHeaderEntry,
                              CRawEntry cpenningBracketEntry,
                              CRawEntry closingBracketEntry,
                              std::vector<CPhdrsStatement>&& phdrsStatements,  
                              std::vector<CRawEntry>&& rawElements,
                              std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_phdrsStatements(std::move(phdrsStatements)),
              m_phdrsHeaderEntry(phdrsHeaderEntry),
              m_phdrsOpenningBracketEntry(phdrsOpenningBracketEntry)
              m_phdrsClosingBracketEntry(phrdsClosingBracketEntry)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::PhdrsRegion;
        }

        /// @brief Reports back PHDR statements
        const std::vector<CPhdrsStatement>& Statements()
        {
            return m_phdrsStatements;
        }

        /// @brief Reports back the entry containing the 'PHDRS' header  
        const CRawEntry PhdrsHeaderEntry()
        {
            return this->m_phdrsHeaderEntry;
        }

        /// @brief Reports back the entry containing the "{" symbol
        const CRawEntry OpenningBracketEntry()
        {
            return this->m_openningBracketEntry;
        }

        /// @brief Reports back the entry containing the "}" symbol
        const CRawEntry ClosingBracketEntry()
        {
            return this->m_closingBracketEntry;
        }
    };
}

#endif
