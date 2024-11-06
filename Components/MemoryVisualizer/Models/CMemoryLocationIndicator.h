#ifndef CMEMORYLOCATIONINDICATOR_H__
#define CMEMORYLOCATIONINDICATOR_H__

#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents the "ENTRY()" directive.
    class CMemoryLocationIndicator : public CContentBase
    {
        DECLARE_STANDARD_PROPERTY(SPointF, LineRightPoint)
        DECLARE_STANDARD_PROPERTY(SPointF, LineLeftPoint)
        DECLARE_STANDARD_PROPERTY(SRectangleF, ContentRectangle)

    public:
        /// @brief Default constructor
        CMemoryLocationIndicator(bool isExternal)
            : CContentBase(false, isExternal)
        {}
    };
}

#endif // CMEMORYLOCATIONINDICATOR_H__
