#ifndef CSTRING_ENTRY_H__
#define CSTRING_ENTRY_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a symbol which is R-Value or L-Value expression and may be preceded by an arithmetic sign.
    class CStringEntry : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_stringEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CStringEntry(const CRawEntry& stringEntry,
                              const std::vector<CRawEntry>& rawElements,
                              const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_stringEntry(stringEntry)
        {}

        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SecondarySymbol;
        }

        /// @brief Gets the symbol itself
        const CRawEntry& StringEntry()
        {
            return this->m_stringEntry;
        }
    };
}

#endif // CSTRING_ENTRY_H__
