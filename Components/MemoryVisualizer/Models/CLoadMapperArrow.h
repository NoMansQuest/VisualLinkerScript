#ifndef CLOADMAPPERARROW_H
#define CLOADMAPPERARROW_H

#include "SPointF.h"
#include "SRectangleF.h"
#include "CLoadMapperArrowCoordinates.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief A laod-mapper links areas where VMA and LMA are separated.
    ///        One exmaple is flash code which is located in ROM (LMA) and
    ///        through CRT-0 operation is loaded into execution region (VMA).
    class CLoadMapperArrow
    {
    public:
        /// @brief Constructor
        CLoadMapperArrow()
        {}

        /// @brief Destructor
        ~CLoadMapperArrow()
        {}

    private:
        CLoadMapperArrowCoordinates m_fixedCoordinates;
        CLoadMapperArrowCoordinates m_draggedCoordinates;
        CLoadMapperArrowCoordinates m_proposedCoordinates;

    public:


    };
};

#endif // CLOADMAPPERARROW_H

