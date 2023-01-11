#ifndef CSECTIONS_REGION_H__
#define CSECTIONS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CSectionsRegion : public CLinkerScriptContentBase
    {       
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_entries;
        CRawEntry m_openningBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_sectionsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionsRegion(CRawEntry sectionsHeaderEntry,
                                 CRawEntry openningBracketEntry,
                                 CRawEntry closingBracketEntry,
                                 std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& entries,
                                 std::vector<CRawEntry>&& rawElements,
                                 std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_entries(std::move(entries)),
              m_openningBracketEntry(openningBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_sectionsHeaderEntry(sectionsHeaderEntry)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsRegion;
        }

        /// @brief Reports back the statements
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Entries()
        {
            return m_entries;
        }

        /// @brief Reports back the entry containing the 'SECTIONS' header
        const CRawEntry SectionsHeaderEntry()
        {
            return this->m_sectionsHeaderEntry;
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
