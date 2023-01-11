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
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_statements;
        CRawEntry m_openningBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_phdrsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CPhdrsRegion(CRawEntry phdrsHeaderEntry,
                              CRawEntry openningBracketEntry,
                              CRawEntry closingBracketEntry,
                              std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& phdrsStatements,
                              std::vector<CRawEntry>&& rawElements,
                              std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_statements(std::move(phdrsStatements)),
              m_openningBracketEntry(openningBracketEntry),
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
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Statements()
        {
            return m_statements;
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
