#ifndef CVERSIONTAG_H__
#define CVERSIONTAG_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CVersionTag : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_tagEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CVersionTag(CRawEntry tagEntry,
                             std::vector<CRawEntry>&& rawElements,
                             std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
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
    };
}

#endif
