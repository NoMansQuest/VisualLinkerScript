#ifndef CCONTENTBASE_H
#define CCONTENTBASE_H

#include "../../../../Helpers.h"
#include <vector>
#include <string>
#include "SRectangleF.h"
#include "EObjectState.h"
#include "CDragState.h"
#include "CTooltip.h"
#include <functional>

/// @brief An enum which serves as a flag for mouse button state
enum class EMouseButtonFlag : uint32_t {
    LeftButton = 1,
    MiddleButton = 2,
    RightButton = 4
};

/// @brief Base class of all memory visualizer models
class CContentBase {
    DECLARE_STANDARD_PROPERTY( bool, IsDraggable )
    DECLARE_STANDARD_PROPERTY( bool, IsFromExternalLinkerScript )
    DECLARE_STANDARD_PROPERTY( bool, IsSelected )
    DECLARE_STANDARD_PROPERTY( bool, IsPressed )
    DECLARE_STANDARD_PROPERTY( bool, IsEnabled )
    DECLARE_STANDARD_PROPERTY( EObjectState, State )
    DECLARE_STANDARD_PROPERTY( SRectangleF, ProposedPosition )
    DECLARE_STANDARD_PROPERTY( CDragState, DragObject )
    DECLARE_STANDARD_PROPERTY( CTooltip, TooltipObject )
    DECLARE_STANDARD_PROPERTY( std::string, Path )
    DECLARE_STANDARD_PROPERTY( uint64_t, HoverStartTimestamp )

    DECLARE_READONLY_PROPERTY( std::function<void(SRectangleF newPosition)>, CurrentPositionUpdatedEvent);

    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, EMouseButtonFlag mouseButtonsState)>, MouseHoverEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, EMouseButtonFlag mouseButtonsState)>, MouseEnteredEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, EMouseButtonFlag mouseButtonsState)>, MouseLeftEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, EMouseButtonFlag mouseButtonsState)>, MouseClickEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, EMouseButtonFlag mouseButtonsState)>, MouseClickDownEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, EMouseButtonFlag mouseButtonsState)>, MouseClickUpEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, SPointF dragStartScreenPosition, SPointF currentCusrsorScreenPosition)>, MouseDragEvent);
    DECLARE_READONLY_PROPERTY( std::function<void(SPointF cursorPositionInComponent, SPointF dragStartScreenPosition, SPointF dropCusrsotScreenPosition)>, MouseDropEvent);

public:
    /// @brief Default Constructor;
    /// @param isDraggable Is the object dragabble?
    /// @param isExternal Is the object present as a result of an 'Include' directive?
    CContentBase(bool isDraggable, bool isExternal) {
        this->m_IsDraggable = isDraggable;
        this->m_IsFromExternalLinkerScript = isExternal;
        this->m_State = EObjectState::Neutral;
    }

private:
    SRectangleF m_currentPosition;

// Functions
public:
    /// @brief Z-Index will be fixed and determined by the caller
    virtual uint8_t ZIndex() = 0;

    /// @brief Returns the current position the component occupies
    SRectangleF CurrentPosition() {
        return this->m_currentPosition;
    }

    /// @brief Updates current position
    void SetCurrentPosition(SRectangleF newPosition) {
        this->m_currentPosition = newPosition;
        if (this->m_CurrentPositionUpdatedEvent) {
            this->m_CurrentPositionUpdatedEvent(newPosition);
        }
    }

    /// @brief Called by the renderer to obtain minmium height this element needs
    virtual float GetMinimumHeight() = 0;

    /// @brief Called by the renderer to obtain minimum width this element needs
    virtual float GetMinimumWidth() = 0;
};

#endif // CCONTENTBASE_H
