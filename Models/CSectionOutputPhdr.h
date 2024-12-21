#ifndef CSECTION_OUTPUT_PHDR_H__
#define CSECTION_OUTPUT_PHDR_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputPhdr : public CParsedContentBase
    {
    private:
        CRawEntry m_phdrName;
        CRawEntry m_colonEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputPhdr(const CRawEntry& colonEntry,
                                    const CRawEntry& phdrRegion,
                                    const std::vector<CRawEntry>& rawElements,
                                    const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_phdrName(phdrRegion),
              m_colonEntry(colonEntry)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputProgramHeader;
        }

        /// @brief Reports back the PHDR regions name.
        [[nodiscard]] const CRawEntry& PhdrRegion()
        {
            return this->m_phdrName;
        }

        /// @brief Reports back the colon initiating the statement.
        [[nodiscard]] const CRawEntry& ColonEntry()
        {
            return this->m_colonEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();
        }

    };
}

#endif
