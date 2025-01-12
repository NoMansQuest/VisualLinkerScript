#ifndef CVERSIONTAG_H__
#define CVERSIONTAG_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a tag found within a Version-Scope in VERSION region
    /// @brief Example: "Something:"
    class CVersionTag : public CParsedContentBase
    {
    private:
        CRawEntry m_tagEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CVersionTag(const CRawEntry& tagEntry,
                             const std::vector<CRawEntry>& rawElements,
                             const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_tagEntry(tagEntry)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::VersionTag;
        }

        /// @brief Reports back the entry containing the 'Tag' itself, which MUST terminate with a colon.
        CRawEntry TagEntry()
        {
            return m_tagEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();
        }
    };
}

#endif
