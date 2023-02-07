#ifndef CSECTION_OUTPUT_AT_LMA_REGION_H__
#define CSECTION_OUTPUT_AT_LMA_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputAtLmaRegion : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_atEntry;
        CRawEntry m_greatherThanSign;
        CRawEntry m_regionName;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputAtLmaRegion(CRawEntry atEntry,
                                CRawEntry greatherThanSign,
                                CRawEntry regionName,
                                std::vector<CRawEntry>&& rawElements,
                                std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_atEntry(atEntry),
              m_greatherThanSign(greatherThanSign),
              m_regionName(regionName)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputAtLmaRegion;
        }

        /// @brief Reports back the "AT" entry
        const CRawEntry& AtEntry()
        {
            return this->m_atEntry;
        }

        /// @brief Reports back the greather-than sign
        const CRawEntry& GreatherThanSign()
        {
            return this->m_greatherThanSign;
        }

        /// @brief Reports back the region name
        const CRawEntry& RegionName()
        {
            return this->m_regionName;
        }
    };
}

#endif
