#ifndef CSECTION_OUTPUT_PHDR_H__
#define CSECTION_OUTPUT_PHDR_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputPhdr : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_phdrName;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputPhdr(CRawEntry phdrRegion,
                                    std::vector<CRawEntry>&& rawElements,
                                    std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_phdrName(phdrRegion)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputPhdr;
        }

        /// @brief Reports back the PHDR Region
        const CRawEntry& PhdrRegion()
        {
            return this->m_phdrName;
        }
    };
}

#endif
