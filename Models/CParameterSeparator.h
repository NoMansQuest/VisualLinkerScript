#ifndef CPARAMETER_SEPARATOR_H__
#define CPARAMETER_SEPARATOR_H__

#include <vector>
#include "CParsedContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a comma which is used to separate parameters in a function
    class CParameterSeparator : public CParsedContentBase
    {
    private:
        CRawEntry m_commaEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CParameterSeparator(const CRawEntry& commaEntry,
                                     const std::vector<CRawEntry>& rawElements,
                                     const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_commaEntry(commaEntry)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::ParameterSeparator;
        }

        /// @brief Gets the symbol itself
        const CRawEntry& CommaEntry()
        {
            return this->m_commaEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] virtual const SharedPtrVector<CViolationBase> AggregateViolation() const {
            return this->Violations();
        }
    };
}

#endif
