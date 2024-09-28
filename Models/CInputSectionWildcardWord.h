#ifndef CINPUTSECTIONWILDCARDWORD_H
#define CINPUTSECTIONWILDCARDWORD_H

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a wildcard word. Example: *.data.?.[A-Za-z]
    class CInputSectionWildcardWord : public CLinkerScriptContentBase
    {
    public:
        /// @brief Default constructor
        explicit CInputSectionWildcardWord(const std::vector<CRawEntry>& composingRawEntries,
                                           const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(composingRawEntries, violations)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionWildcardWord;
        }
    };
}

#endif // CINPUTSECTIONWILDCARDWORD_H
