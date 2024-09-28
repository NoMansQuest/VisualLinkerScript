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
        SharedPtrVector<CLinkerScriptContentBase> m_versionNodes;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_versionHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CVersionsRegion(const CRawEntry& versionHeaderEntry,
                                 const CRawEntry& openingBracketEntry,
                                 const CRawEntry& closingBracketEntry,
                                 const SharedPtrVector<CLinkerScriptContentBase>& versionNodes,
                                 const std::vector<CRawEntry>& rawElements,
                                 const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_versionNodes(versionNodes),
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
        const SharedPtrVector<CLinkerScriptContentBase>& Nodes()
        {
            return m_versionNodes;
        }

        /// @brief Reports back the entry containing the 'VERSION' header
        [[nodiscard]] CRawEntry VersionHeaderEntry()
        {
            return this->m_versionHeaderEntry;
        }

        /// @brief Reports back the entry containing the "{" symbol
        [[nodiscard]] CRawEntry OpeningBracketEntry()
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the entry containing the "}" symbol
        [[nodiscard]] CRawEntry ClosingBracketEntry()
        {
            return this->m_closingBracketEntry;
        }
    };
}

#endif
