#ifndef CLMATOVMAARROW_H
#define CLMATOVMAARROW_H

#include "CLmaToVmaArrowCoordinates.h"
#include "CContentBase.h"
#include "EObjectState.h"
#include <memory>

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
        CLmaToVmaArrowCoordinates m_fixedCoordinates;
        CLmaToVmaArrowCoordinates m_draggedCoordinates;
        CLmaToVmaArrowCoordinates m_proposedCoordinates;

        std::shared_ptr<CContentBase> m_lmaObject;
        std::shared_ptr<CContentBase> m_vmaObject;

        EObjectState m_state;

    public:        
        /// @brief Reports back the state of the object.
        EObjectState State() {
            return this->m_state;
        }

        /// @brief Updates the state of the object.
        void SetState(EObjectState state){
            this->m_state = state;
        }

        /// @brief Reports back the fixed-coordinates
        CLmaToVmaArrowCoordinates& FixedCoordinates() {
            return this->m_fixedCoordinates;
        }

        /// @brief Reports back the 'Dragged' coordinates of the object
        CLmaToVmaArrowCoordinates& DraggedCoordinates() {
            return this->m_draggedCoordinates;
        }

        /// @brief Reports back the 'Proposed' coordinates of the object
        CLmaToVmaArrowCoordinates& ProposedCoordinates() {
            return this->m_proposedCoordinates;
        }

        /// @brief Reports back the object attached to the 'LMA' side of the arrow
        std::shared_ptr<CContentBase> LmaObject() {
            return this->m_lmaObject;
        }

        /// @brief Updates the object on the 'LMA' side of the arrow
        void SetLmaObject(std::shared_ptr<CContentBase> lmaObject){
            this->m_lmaObject = lmaObject;
        }

        /// @brief Reports back the object attached to the 'VMA' side of the arrow
        std::shared_ptr<CContentBase> VmaObject() {
            return this->m_vmaObject;
        }

        /// @brief Updates the object on the 'VMA' side of the arrow
        void SetVmaObject(std::shared_ptr<CContentBase> vmaObject){
            this->m_vmaObject = vmaObject;
        }
    };
};

#endif // CLMATOVMAARROW_H

