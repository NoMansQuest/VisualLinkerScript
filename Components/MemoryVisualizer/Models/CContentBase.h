#ifndef CCONTENTBASE_H
#define CCONTENTBASE_H

#include "../../../../Helpers.h"

#include <vector>
#include <string>
#include "SRectangleF.h"
#include "EObjectState.h"
#include "CDragState.h"
#include "CTooltip.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Base class of all memory visualizer models
    class CContentBase
    {        
        DECLARE_STANDARD_PROPERTY( bool, IsDraggable )
        DECLARE_STANDARD_PROPERTY( bool, IsFromExternalLinkerScript )
        DECLARE_STANDARD_PROPERTY( bool, IsSelected )
        DECLARE_STANDARD_PROPERTY( bool, IsPressed )
        DECLARE_STANDARD_PROPERTY( bool, IsEnabled )
        DECLARE_STANDARD_PROPERTY( EObjectState, State )
        DECLARE_STANDARD_PROPERTY( SRectangleF, ProposedCoordinatesDuringHover )
        DECLARE_STANDARD_PROPERTY( SRectangleF, FixedCoordinates )
        DECLARE_STANDARD_PROPERTY( CDragState, DragObject )
        DECLARE_STANDARD_PROPERTY( CTooltip, TooltipObject )
        DECLARE_STANDARD_PROPERTY( std::string, Path )
        DECLARE_STANDARD_PROPERTY( uint64_t, HoverStartTimestamp )

    public:
        /// @brief Default Constructor;
        /// @param isDraggable Is the object dragabble?
        /// @param isExternal Is the object present as a result of an 'Include' directive?
        CContentBase(bool isDraggable, bool isExternal)
        {
            this->m_IsDraggable = isDraggable;
            this->m_IsFromExternalLinkerScript = isExternal;
            this->m_State = EObjectState::Neutral;
        }

        /// @brief Destructor
        ~CContentBase()
        {}

    // Functions
    public:
        /// @brief Z-Index will be fixed and determined by the caller
        virtual uint8_t ZIndex() = 0;    
    };
};

#endif // CCONTENTBASE_H
