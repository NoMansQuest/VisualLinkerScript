#ifndef CLMATOVMAARROW_H
#define CLMATOVMAARROW_H

#include "SPointF.h"
#include "SRectangleF.h"
#include "CLmaToVmaArrowCoordinates.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief A laod-mapper links areas where VMA and LMA are separated.
    ///        One exmaple is flash code which is located in ROM (LMA) and
    ///        through CRT-0 operation is loaded into execution region (VMA).
    class CLmaToVmaArrow
    {
    public:
        /// @brief Constructor
        CLmaToVmaArrow()
        {}

        /// @brief Destructor
        ~CLmaToVmaArrow()
        {}

    private:
        CLoadMapperArrowCoordinates m_fixedCoordinates;
        CLoadMapperArrowCoordinates m_draggedCoordinates;
        CLoadMapperArrowCoordinates m_proposedCoordinates;

    public:


    };
};

#endif // CLMATOVMAARROW_H

