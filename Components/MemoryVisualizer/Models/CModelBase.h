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
        ~CLinkerScriptObject()
        {}

    // Member fields
    private:
        bool m_isHoveringOver;
        uint64_t m_hoverStartTimestamp;

        bool m_isSelected;
        bool m_isPressed;
        bool m_isEnabled;

        SRectangleF m_coordinates;
        CModelDrag m_dragObject;
        CModelToolTip m_tooltipObject;        

    // Functions
    public:
    
        /// @brief Return current coordinates.
        SRectangleF Coordinates() 
        { 
            return this->Coordinates; 
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

        /// @brief Report back the tool-tip content
        std::string ToolTipContent() { return this->m_toolTipContent; }
    };
};

#endif // CMODELBASE_H
