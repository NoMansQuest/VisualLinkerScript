#ifndef CMODELBASE_H
#define CMODELBASE_H

#include <vector>
#include <string>
#include "SPointF.h"
#include "SRectangleF.h"
#include "CModelDrag.h"
#include "CModelTooltip.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Base class of all memory visualizer models
    class CModelBase 
    {
    public:
        /// @brief Default Constructor;
        CModelBase()
        {}

        /// @brief Destructor
        ~CModelBase()
        {}

    // Member fields
    private:
        bool m_isHoveringOver;
        uint64_t m_hoverStartTimestamp;

        bool m_isSelected;
        bool m_isPressed;
        bool m_isEnabled;

        SRectangleF m_fixedCoordinates;
        SRectangleF m_hoveringCoordinates;
        bool m_isBeingDragged
        CModelDrag m_dragObject;
        CModelToolTip m_tooltipObject;     
        std::string m_objectPath;   

    // Functions
    public:
    
        /// @brief Return current fixed coordinates.
        SRectangleF FixedCoordinates() 
        { 
            return this->m_fixedCoordinates; 
        }

        /// @brief Return hovering coordinates (if hovering).
        SRectangleF HoveringCoordinates() 
        { 
            return this->m_hoveringCoordinates; 
        }

        /// @brief Update coordinates
        void SetCoordinates(SRectangleF newCoordinates) 
        { 
            this->m_coordinates = newCoordinates; 
        }    

        /// @brief Returns back the 'Drag' data object.
        CModelDragData& DragObject() { return this->m_dragData; }

        /// @brief Returns back the Tool-Tip object
        CModelToolTipData& TooltipObject() { return this->m_tooltipObject; }

        /// @brief When did the mouse enter this control (mainly used for ToolTip management)
        uint64_t HoverStartTimestamp() { return this->m_hoverStartTimestamp; };

        /// @brief Report is the object is selected
        bool IsSelected() { return this->m_isSelected; }

        /// @brief Report if the object is pressed.
        bool IsPressed() { return this->m_isPressed; }

        /// @brief Report if the object is enabled.
        bool IsEnabled() { return this->m_isEnabled; }

        /// @brief Reports back if the object is being dragged
        bool IsBeingDragged() { return this->m_dragObject.IsBeingDragged(); }

        /// @brief Reports back the path of the object
        const std::string& ObjectPath() { return this->m_objectPath; }

        /// @brief Sets the object path
        void SetObjectPath(std::string bewobjectPath);

    signals:


    };
};

#endif // CMODELBASE_H
