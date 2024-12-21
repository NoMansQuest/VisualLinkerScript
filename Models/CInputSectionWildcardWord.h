#ifndef CINPUTSECTIONWILDCARDWORD_H
#define CINPUTSECTIONWILDCARDWORD_H

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a wildcard word. Example: *.data.?.[A-Za-z]
    class CInputSectionWildcardWord : public CParsedContentBase
    {
    public:
        /// @brief Default constructor
        explicit CInputSectionWildcardWord(const std::vector<CRawEntry>& composingRawEntries,
                                           const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(composingRawEntries, violations)
        {}

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override
        {
            return this->Violations();
        }

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionWildcardWord;
        }
    };
}

#endif // CINPUTSECTIONWILDCARDWORD_H
