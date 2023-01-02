#ifndef CVERSION_NODE_H__
#define CVERSION_NODE_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CVersionNode : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CVersionNode(std::vector<CRawEntry>&& composingRawElements,                              
                              std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(composingRawElements, violations)
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::VersionNode;
        }
    };
}

#endif
