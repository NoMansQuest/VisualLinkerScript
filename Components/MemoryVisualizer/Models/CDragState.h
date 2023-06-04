#ifndef CDRAGSTATE_H__
#define CDRAGSTATE_H__

#include <vector>
#include <string>
#include <cstdint>
#include "SPointF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Drag data shared across models.
    class CDragState
    {
    private:
        bool m_isBeingDragged;
        SPointF m_dragStartCursorPosition;
        SPointF m_dragCurrentPosition;

    public:
        /// @brief Returns if the object is being dragged
        float IsBeingDragged() {
            return this->m_isBeingDragged;
        }

        /// @brief Returns the cursor's position when the drag was initiated
        SPointF DragStartCursorPosition() {
            return this->m_dragStartCursorPosition;
        }

        /// @brief Returns the current hovering coordinates of the object
        SPointF DragCurrentPosition() {
            return this->m_dragCurrentPosition;
        }

        /// @brief Updates the drag state of the object
        /// @param isBeingDragged If true, the object is being dragged
        void SetDragState(bool isBeingDragged) {
            this->m_isBeingDragged = isBeingDragged;
        }

        /// @brief Updates drag start position (position of the mouse where Drag started)
        /// @param cursorStartPos Position of the cursor initiating the drag action
        void SetDragStartPosition(SPointF cursorStartPosition) {
            this->m_dragStartCursorPosition = cursorStartPosition;
        }

        /// @brief Updates drag position (where the object is hover in the air)
        /// @param xPos Current position of the object while it's in the air
        void SetCurrentDragPosition(SPointF dragCurrentPosition) {
            this->m_dragCurrentPosition = dragCurrentPosition;
        }
    };
}

#endif // CDRAGSTATE_H__
