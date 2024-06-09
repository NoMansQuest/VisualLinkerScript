#ifndef CLMATOVMAARROW_H
#define CLMATOVMAARROW_H

#include "../../../Helpers.h"

#include "CLmaToVmaArrowCoordinates.h"
#include "CContentBase.h"
#include "EObjectState.h"
#include <memory>

/// @brief A laod-mapper links areas where VMA and LMA are separated.
///        One exmaple is flash code which is located in ROM (LMA) and
///        through CRT-0 operation is loaded into execution region (VMA).
class CLmaToVmaArrow
{
    DECLARE_STANDARD_PROPERTY(CLmaToVmaArrowCoordinates, FixedCoordinates )
    DECLARE_STANDARD_PROPERTY(CLmaToVmaArrowCoordinates, DraggedCoordinates )
    DECLARE_STANDARD_PROPERTY(CLmaToVmaArrowCoordinates, ProposedCoordinates )
    DECLARE_STANDARD_PROPERTY(std::shared_ptr<CContentBase>, LmaObject )
    DECLARE_STANDARD_PROPERTY(std::shared_ptr<CContentBase>, VmaObject )
    DECLARE_STANDARD_PROPERTY(EObjectState, State )
};

#endif // CLMATOVMAARROW_H

