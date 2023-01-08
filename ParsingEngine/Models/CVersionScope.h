#ifndef CVERSION_SCOPE_H__
#define CVERSION_SCOPE_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single entry in a version-node inside the "VERSIONS" region
    class CVersionScope : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CVersionScope(std::vector<CRawEntry>&& rawElements,
                               std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations))
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::VersionScope;
        }
    };
}

#endif
