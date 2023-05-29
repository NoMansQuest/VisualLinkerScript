#ifndef CCONTENTBASE_H
#define CCONTENTBASE_H

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
    public:
        /// @brief Default Constructor;
        CContentBase(bool isDraggable)
            : m_isDraggable(isDraggable),
              m_state(EObjectState::Neutral)
        {}

        /// @brief Destructor
        ~CContentBase()
        {}

    // Member fields
    private:        
        uint64_t m_hoverStartTimestamp;

        bool m_isSelected;
        bool m_isPressed;
        bool m_isEnabled;
        bool m_isDraggable;

        SRectangleF m_fixedCoordinates;
        SRectangleF m_proposedCoordinates;
        CDragState m_dragObject;
        CTooltip m_tooltipObject;
        std::string m_path;
        EObjectState m_state;

    // Functions
    public:
        /// @brief Z-Index will be fixed and determined by the caller
        virtual uint8_t ZIndex() = 0;
    
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
        CDragState& DragObject() {
            return this->m_dragObject;
        }

        /// @brief Returns back the 34618Tool-Tip object
        CTooltip& TooltipObject() {
            return this->m_tooltipObject;
        }

        /// @brief When did the mouse enter this control (mainly used for ToolTip management)
        uint64_t HoverStartTimestamp() {
            return this->m_hoverStartTimestamp;
        }

        /// @brief Sets the Hover Start Timestamp
        void SetHoverStartTimestamp(uint64_t hoverStartTimestamp){
            this->m_hoverStartTimestamp = hoverStartTimestamp;
        }

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

        /// @brief Sets the object path. Path could be updated as object moves
        void SetObjectPath(std::string newPath){
            this->m_path = newPath;
        }

        /// @brief Reports back whether the object is draggable or fixed
        bool IsDraggable(){
            return this->m_isDraggable;
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

#endif // CCONTENTBASE_H
