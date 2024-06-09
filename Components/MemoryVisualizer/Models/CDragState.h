#ifndef CDRAGSTATE_H__
#define CDRAGSTATE_H__

#include "../../../../Helpers.h"

#include <vector>
#include <string>
#include <cstdint>
#include "SPointF.h"

/// @brief Drag data shared across models.
class CDragState
{
    DECLARE_STANDARD_PROPERTY( bool, IsBeingDragged )
    DECLARE_STANDARD_PROPERTY( SPointF, DragStartCursorPosition )
    DECLARE_STANDARD_PROPERTY( SPointF, DragCurrentPosition )
};

#endif // CDRAGSTATE_H__
