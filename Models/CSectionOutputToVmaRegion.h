#ifndef CSECTION_OUTPUT_TO_REGION_H__
#define CSECTION_OUTPUT_TO_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputToVmaRegion : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_greatherThanSign;
        CRawEntry m_regionName;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputToVmaRegion(const CRawEntry& greaterThanSign,
                                           const CRawEntry& regionName,
                                           const std::vector<CRawEntry>& rawElements,
                                           const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_greatherThanSign(greaterThanSign),
              m_regionName(regionName)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputToRegion;
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
    };
}

#endif
