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

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of
        /// @param phdrsStatements A list of PHDR statements
        /// @param violations Violations found in the current element
        explicit CPhdrsRegion(std::vector<CRawEntry>&& rawElements, 
                              std::vector<CPhdrsStatement>&& phdrsStatements,  
                              std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_phdrsStatements(std::move(phdrsStatements))
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
    }
}

#endif