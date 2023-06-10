#ifndef CVERSIONS_REGION_H__
#define CVERSIONS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents the VERSIONS region in the Linker-Script.
    class CVersionsRegion : public CLinkerScriptContentBase
    {  
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& m_versionNodes;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_versionHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of
        /// @param versionNodes A list of version nodes
        /// @param violations Violations found in the current element
        explicit CVersionsRegion(CRawEntry versionHeaderEntry,
                                 CRawEntry openingBracketEntry,
                                 CRawEntry closingBracketEntry,
                                 std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& versionNodes,
                                 std::vector<CRawEntry>&& rawElements,
                                 std::vector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_versionNodes(std::move(versionNodes)),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_versionHeaderEntry(versionHeaderEntry)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::PhdrsRegion;
        }

        /// @brief Reports back PHDR statements
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Nodes()
        {
            return m_versionNodes;
        }

        /// @brief Reports back the entry containing the 'VERSION' header
        const CRawEntry VersionHeaderEntry()
        {
            return this->m_versionHeaderEntry;
        }

        /// @brief Reports back the entry containing the "{" symbol
        const CRawEntry OpeningBracketEntry()
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the entry containing the "}" symbol
        const CRawEntry ClosingBracketEntry()
        {
            return this->m_closingBracketEntry;
        }
    };
}

#endif
