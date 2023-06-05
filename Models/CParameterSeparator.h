#ifndef CPARAMETER_SEPARATOR_H__
#define CPARAMETER_SEPARATOR_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a comma which is used to separate parameters in a function
    class CParameterSeparator : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_commaEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CParameterSeparator(CRawEntry commaEntry,
                              std::vector<CRawEntry>&& rawElements,
                              std::vector<CParserViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
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
    };
}

#endif
