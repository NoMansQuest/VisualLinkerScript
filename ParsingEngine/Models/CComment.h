#ifndef CCOMMENT_H__
#define CCOMMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a comment entry
    class CComment : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CComment(std::vector<CRawEntry>&& composingRawElements,                              
                          std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(composingRawElements), std::move(violations))
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Comment;
        }
    };
}

#endif
