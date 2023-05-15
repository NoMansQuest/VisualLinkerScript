#ifndef CMODELBASE_H
#define CMODELBASE_H

#include <vector>
#include <string>
#include "SPointF.h"
#include "SRectangleF.h"
#include "EObjectState.h"
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
        SRectangleF m_proposedCoordinates;
        CModelDrag m_dragObject;
        CModelTooltip m_tooltipObject;
        std::string m_path;
        EObjectState m_state;

    // Functions
    public:
    
        /// @brief Return current fixed coordinates.
        SRectangleF FixedCoordinates() {
            return this->m_fixedCoordinates; 
        }

        /// @brief Update coordinates
        void SetFixedCoordinates(SRectangleF newCoordinates) {
            this->m_fixedCoordinates = newCoordinates;
        }    

        /// @brief Return current proposed coordinates.
        SRectangleF ProposedCoordinates() {
            return this->m_proposedCoordinates;
        }

        /// @brief Update coordinates
        void SetProposedCoordinates(SRectangleF newCoordinates) {
            this->m_proposedCoordinates = newCoordinates;
        }

        /// @brief Returns back the 'Drag' data object.
        CModelDrag& DragObject() {
            return this->m_dragObject;
        }

        /// @brief Returns back the Tool-Tip object
        CModelTooltip& TooltipObject() {
            return this->m_tooltipObject;
        }

        /// @brief When did the mouse enter this control (mainly used for ToolTip management)
        uint64_t HoverStartTimestamp() {
            return this->m_hoverStartTimestamp;
        };

        /// @brief Report is the object is selected
        bool IsSelected() {
            return this->m_isSelected;
        }

        /// @brief Report if the object is pressed.
        bool IsPressed() {
            return this->m_isPressed;
        }

        /// @brief Report if the object is enabled.
        bool IsEnabled() {
            return this->m_isEnabled;
        }

        /// @brief Reports back the path of the object
        const std::string& Path() {
            return this->m_path;
        }

        /// @brief Sets the object path
        void SetObjectPath(std::string newPath){
            this->m_path = newPath;
        }

        /// @brief Reports back current object state
        EObjectState State() {
            return this->m_state;
        }

        /// @brief Updates the state.
        void SetState(EObjectState newState) {
            this->m_state = newState;
        }
    };
};

#endif // CMODELBASE_H
