#ifndef CSECTION_OVERLAY_H__
#define CSECTION_OVERLAY_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOverlayStatement : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CSectionOverlayStatement(std::vector<CRawEntry>&& composingRawElements, 
                                          std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(composingRawElements), std::move(violations))
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
        }
    };
}

#endif
