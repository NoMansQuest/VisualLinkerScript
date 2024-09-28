#ifndef CCOMMENT_H__
#define CCOMMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a comment entry
    class CComment : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        /// @param violations Violations list.
        explicit CComment(const std::vector<CRawEntry>& composingRawElements,                              
                          const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(composingRawElements, violations)
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Comment;
        }
    };
}

#endif
