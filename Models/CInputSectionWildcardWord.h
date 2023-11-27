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
        explicit CInputSectionWildcardWord(std::vector<CRawEntry> composingRawEntries,
                                           SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(composingRawEntries), std::move(violations))
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionWildcardWord;
        }
    };
}

#endif // CINPUTSECTIONWILDCARDWORD_H
