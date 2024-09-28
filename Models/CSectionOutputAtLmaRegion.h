#ifndef CSECTION_OUTPUT_AT_LMA_REGION_H__
#define CSECTION_OUTPUT_AT_LMA_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputAtLmaRegion : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_atEntry;
        CRawEntry m_greatherThanSign;
        CRawEntry m_regionName;

    public:
        /// @brief Default constructor
        explicit CSectionOutputAtLmaRegion(
							    const CRawEntry& atEntry,
                                const CRawEntry& greaterThanSign,
                                const CRawEntry& regionName,
                                const std::vector<CRawEntry>& rawElements,
                                const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_atEntry(atEntry),
              m_greatherThanSign(greaterThanSign),
              m_regionName(regionName)
        {}

        /// @brief Alternate constructor without 'rawElements'
        explicit CSectionOutputAtLmaRegion(
								const CRawEntry& atEntry,
                                const CRawEntry& greaterThanSign,
                                const CRawEntry& regionName,
                                const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase({atEntry, greaterThanSign, regionName}, violations),
              m_atEntry(atEntry),
              m_greatherThanSign(greaterThanSign),
              m_regionName(regionName)
        {}

        /// @brief Alternate constructor without 'rawElements' and 'violations'
        explicit CSectionOutputAtLmaRegion(
								const CRawEntry& atEntry,
                                const CRawEntry& greaterThanSign,
                                const CRawEntry& regionName)
            : CLinkerScriptContentBase({atEntry, greaterThanSign, regionName}, {}),
              m_atEntry(atEntry),
              m_greatherThanSign(greaterThanSign),
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
