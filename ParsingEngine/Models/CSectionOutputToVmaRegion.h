#ifndef CSECTION_OUTPUT_TO_REGION_H__
#define CSECTION_OUTPUT_TO_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputToVmaRegion : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_greatherThanSign;
        CRawEntry m_regionName;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputToVmaRegion(CRawEntry greatherThanSign,
                                           CRawEntry regionName,
                                           std::vector<CRawEntry>&& rawElements,
                                           std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_greatherThanSign(greatherThanSign),
              m_regionName(regionName)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputToRegion;
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
