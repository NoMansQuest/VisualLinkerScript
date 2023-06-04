#ifndef CVERSION_SCOPE_ENTRY_H__
#define CVERSION_SCOPE_ENTRY_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single version entry placed under a scope (or directly) the "VERSIONS" region
    class CVersionScopeEntry : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CVersionScopeEntry(std::vector<CRawEntry>&& rawElements,
                                    std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations))
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::VersionScopeEntry;
        }
    };
}

#endif
