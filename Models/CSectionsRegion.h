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
        SharedPtrVector<CLinkerScriptContentBase> m_entries;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_sectionsHeaderEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionsRegion(const CRawEntry& sectionsHeaderEntry,
                                 const CRawEntry& openingBracketEntry,
                                 const CRawEntry& closingBracketEntry,
                                 const SharedPtrVector<CLinkerScriptContentBase>& entries,
                                 const std::vector<CRawEntry>& rawElements,
                                 const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_entries(entries),
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
        [[nodiscard]] const SharedPtrVector<CLinkerScriptContentBase>& Entries() const
        {
            return m_entries;
        }

        /// @brief Reports back the entry containing the 'SECTIONS' header
        [[nodiscard]] CRawEntry SectionsHeaderEntry() const
        {
            return this->m_sectionsHeaderEntry;
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

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
