#ifndef COVERLAYOBJECT_H__
#define COVERLAYOBJECT_H__

#include <vector>
#include <string>
#include <cstdint>
#include "../../../Helpers.h"
#include "CContentBase.h"
#include "COverlaySectionObject.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents an 'Overlay' object found in "SECTION"
    class COverlayObject : CContentBase
    {
        DECLARE_READONLY_PROPERTY( std::string VmalRegion, ProgramHeaders )
        DECLARE_READONLY_PROPERTY( SharedPtrVector<COverlaySectionObject>, ChildContent )
        DECLARE_READONLY_PROPERTY( std::vector<std::string>, ProgramHeaders )
    };
};


#endif // CARROWOBJECT_H__
