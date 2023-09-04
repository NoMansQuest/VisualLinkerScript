#ifndef CSECTION_OUTPUT_PHDR_H__
#define CSECTION_OUTPUT_PHDR_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputPhdr : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_phdrName;
        CRawEntry m_colonEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputPhdr(CRawEntry colonEntry,
                                    CRawEntry phdrRegion,
                                    std::vector<CRawEntry>&& rawElements,
                                    SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_phdrName(phdrRegion),
              m_colonEntry(colonEntry)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputPhdr;
        }

        /// @brief Reports back the PHDR regions name.
        const CRawEntry& PhdrRegion()
        {
            return this->m_phdrName;
        }

        /// @brief Reports back the colon initiating the statement.
        const CRawEntry& ColonEntry()
        {
            return this->m_colonEntry;
        }
    };
}

#endif
