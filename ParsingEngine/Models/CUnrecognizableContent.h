#ifndef CUNRECOGNIZABLE_CONTENT_H__
#define CUNRECOGNIZABLE_CONTENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CUnrecognizableContent : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Construct an "Unrecognizable" object
        explicit CUnrecognizableContent(std::vector<CRawEntry>&& composingRawElements, 
                                        std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(composingRawElements, violations)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::UnrecognizableContent;
        }
    };
}


#endif
