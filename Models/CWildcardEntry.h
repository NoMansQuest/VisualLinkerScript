#ifndef CWILDCARDENTRY_H
#define CWILDCARDENTRY_H

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a symbol which is R-Value or L-Value expression and may be preceded by an arithmetic sign.
    class CWildcardEntry : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_wildcardEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CWildcardEntry(CRawEntry wildcardEntry,
                                std::vector<CRawEntry>&& rawElements,
                                SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_wildcardEntry(wildcardEntry)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::WildcardEntry;
        }

        /// @brief Gets the symbol itself
        const CRawEntry& WildcardEntry()
        {
            return this->m_wildcardEntry;
        }
    };
}

#endif // CWILDCARDENTRY_H
