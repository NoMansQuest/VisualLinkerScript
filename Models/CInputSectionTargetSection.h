#ifndef CINPUT_SECTION_TARGET_SECTION_H__
#define CINPUT_SECTION_TARGET_SECTION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Input-Section Target
    /// @brief Example: The '.data.' in '*(.data)
    class CInputSectionTargetSection : public CLinkerScriptContentBase
    {
    public:
        /// @brief Default constructor
        explicit CInputSectionTargetSection(CRawEntry targetSection,
                                            SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase({targetSection}, std::move(violations))
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionTargetSection;
        }
    };
}


#endif
