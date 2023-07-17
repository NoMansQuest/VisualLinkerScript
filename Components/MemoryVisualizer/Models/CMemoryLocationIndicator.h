#ifndef CMEMORYLOCATIONINDICATOR_H__
#define CMEMORYLOCATIONINDICATOR_H__

#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents the "ENTRY()" directive.
    class CMemoryLocationIndicator : public CContentBase
    {
        DECLARE_STANDARD_PROPERTY( SPointF, RightSidePosition )
        DECLARE_STANDARD_PROPERTY( SPointF, LeftSidePoition )
        DECLARE_STANDARD_PROPERTY( SRectangleF, ContentRectangle )
    };
};

#endif // CMEMORYLOCATIONINDICATOR_H__
