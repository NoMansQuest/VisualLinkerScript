#ifndef CSECTIONS_REGION_H__
#define CSECTIONS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CSectionsRegion : public CLinkerScriptContentBase
    {       
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_entries;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_sectionsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionsRegion(CRawEntry sectionsHeaderEntry,
                                 CRawEntry openingBracketEntry,
                                 CRawEntry closingBracketEntry,
                                 std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& entries,
                                 std::vector<CRawEntry>&& rawElements,
                                 SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_entries(std::move(entries)),
              m_openingBracketEntry(openingBracketEntry),
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
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Entries() const
        {
            return m_entries;
        }

        /// @brief Reports back the entry containing the 'SECTIONS' header
        const CRawEntry SectionsHeaderEntry() const
        {
            return this->m_sectionsHeaderEntry;
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
