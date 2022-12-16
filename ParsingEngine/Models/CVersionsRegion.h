#ifndef CVERSIONS_REGION_H__
#define CVERSIONS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the VERSIONS region in the Linker-Script.
    class CVersionsRegion : public CLinkerScriptContentBase
    {  
    private:
        std::vector<CVersionNode> m_versionStatements;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of
        /// @param versionNodes A list of version nodes
        /// @param violations Violations found in the current element
        explicit CVersionsRegion(std::vector<CRawEntry>&& rawElements, 
                                 std::vector<CVersionNode>&& versionNodes,  
                                 std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_versionStatements(std::move(versionNodes))
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