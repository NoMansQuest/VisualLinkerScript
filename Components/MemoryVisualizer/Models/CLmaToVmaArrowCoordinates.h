#ifndef CLMATOVMAARROWCOORDINATES_H
#define CLMATOVMAARROWCOORDINATES_H

#include "../../../Helpers.h"

#include "SPointF.h"
#include "SRectangleF.h"

/// @brief Represents full coordinates of a load-mapper aarrow
class CLmaToVmaArrowCoordinates
{
    DECLARE_STANDARD_PROPERTY(SPointF, VmaSideCoordinates)
    DECLARE_STANDARD_PROPERTY(SPointF, LmaSideCoordinates)
    DECLARE_STANDARD_PROPERTY(SRectangleF, VmaSidePointerRegion)
    DECLARE_STANDARD_PROPERTY(SRectangleF, LmaSidePointerRegion)
    DECLARE_STANDARD_PROPERTY(int, AllocatingVerticalSlot)
};

#endif // CLMATOVMAARROWCOORDINATES_H
