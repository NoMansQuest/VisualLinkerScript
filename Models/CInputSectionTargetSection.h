#ifndef CINPUT_SECTION_TARGET_SECTION_H__
#define CINPUT_SECTION_TARGET_SECTION_H__

#include <vector>
#include "CParsedContentBase.h"
#include "CInputSectionWildcardWord.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Input-Section Target
    /// @brief Example: The '.data.' in '*(.data)
    class CInputSectionTargetSection : public CParsedContentBase
    {
    public:
        /// @brief Default constructor
        explicit CInputSectionTargetSection(const CInputSectionWildcardWord& targetSection,
                                            const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(targetSection.RawEntries(), violations)
        {}

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override
        {
            return this->Violations();
        }

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionTargetSection;
        }
    };
}

#endif
