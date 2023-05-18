#ifndef CLMATOVMAARROWCOORDINATES_H
#define CLMATOVMAARROWCOORDINATES_H

#include "SPointF.h"
#include "SRectangleF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents full coordinates of a load-mapper aarrow
    class CLmaToVmaArrowCoordinates
    {
    public:
        /// @brief Constructor
        CLmaToVmaArrowCoordinates()
        {}

        /// @brief Destructor
        ~CLmaToVmaArrowCoordinates()
        {}

    private:
        SPointF m_vmaSideCoordinates;
        SPointF m_lmaSideCoordinates;
        SRectangleF m_vmaSidePointerRegion;
        SRectangleF m_lmaSidePointerRegion;
        int m_allocatingVerticalSlot;

    public:

        /// @brief Reports back the coordinate of the VMA side
        SPointF VmaSideCoordinates() {
            return this->m_vmaSideCoordinates;
        }

        /// @brief Updates VMA-Side coordinates
        void SetVmaSideCoordinates(SPointF newCoordinates) {
            this->m_vmaSideCoordinates = newCoordinates;
        }

        /// @brief Reports back the coordinate of the LMA side
        SPointF LmaSideCoordinates() {
            return this->m_lmaSideCoordinates;
        }

        /// @brief Updates VMA-Side coordinates
        void SetLmaSideCoordinates(SPointF newCoordinates) {
            this->m_lmaSideCoordinates = newCoordinates;
        }

        /// @brief Reports back the rectangle where VMA-sides symbol is residing (hence responsive to mouse-drag)
        SRectangleF VmaSidePointerRegion() {
            return this->m_vmaSidePointerRegion;
        }

        /// @brief Sets VMA-Side pointer region
        void SetVmaSidePointerRegion(SRectangleF regionRectangle) {
            this->m_vmaSidePointerRegion = regionRectangle;
        }

        /// @brief Reports back the rectangle where LMA-sides symbol is residing (hence responsive to mouse-drag)
        SRectangleF LmaSidePointerRegion() {
            return this->m_lmaSidePointerRegion;
        }

        /// @brief Sets LMA-Side pointer region
        void SetLmaSidePointerRegion(SRectangleF regionRectangle) {
            this->m_lmaSidePointerRegion = regionRectangle;
        }

        /// @brief Reports back the index of the vertical slot allocated.
        int AllocatingVerticalSlot() {
            return this->m_allocatingVerticalSlot;
        }
    };
};

#endif // CLMATOVMAARROWCOORDINATES_H
