#ifndef CSECTION_OUTPUT_AT_LMA_REGION_H__
#define CSECTION_OUTPUT_AT_LMA_REGION_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputAtLmaRegion : public CParsedContentBase
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
            : CParsedContentBase(rawElements, violations),
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
            : CParsedContentBase({atEntry, greaterThanSign, regionName}, violations),
              m_atEntry(atEntry),
              m_greatherThanSign(greaterThanSign),
              m_regionName(regionName)
        {}

        /// @brief Alternate constructor without 'rawElements' and 'violations'
        explicit CSectionOutputAtLmaRegion(
								const CRawEntry& atEntry,
                                const CRawEntry& greaterThanSign,
                                const CRawEntry& regionName)
            : CParsedContentBase({atEntry, greaterThanSign, regionName}, {}),
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
        const CRawEntry& GreaterThanSign()
        {
            return this->m_greatherThanSign;
        }

        /// @brief Reports back the region name
        const CRawEntry& RegionName()
        {
            return this->m_regionName;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();	        
        }
    };
}

#endif
