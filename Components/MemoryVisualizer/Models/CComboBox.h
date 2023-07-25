#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include <vector>
#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Base class of all memory visualizer models
    class CComboBox : public CContentBase {
        DECLARE_READONLY_PROPERTY( std::function<void(int32_t)>, SelectedIndexChangedEvent )

    public:
        /// @brief Default Constructor;
        /// @param isDraggable Is the object dragabble?
        /// @param isExternal Is the object present as a result of an 'Include' directive?
        CComboBox(bool isDraggable, bool isExternal) :
            CContentBase(isDraggable, isExternal)
        {}

    private:
        std::vector<std::string> m_possibleValues;
        std::int32_t m_selectedIndex;
        std::string m_selectedValue;

    public:
        /// @brief Returns back the list of possible values (returned by value since the
        ///        list can't be that large)
        std::vector<std::string> PossibleValues() {
            return this->m_possibleValues;
        }

        /// @brief Return currently selected value
        std::string SelectedValue() {
            if ((m_selectedIndex == -1) || (m_possibleValues.empty())){
                return "";
            }
            return m_possibleValues[m_selectedIndex];
        }

        /// @brief Returns back the current selected index.
        int32_t SelectedIndex(){
            return this->m_selectedIndex;
        }

        /// @brief Sets the 'Selected Index'
        void SetSelectedIndex(int32_t index){
            if ((index > m_possibleValues.size() - 1) || (index < - 1)) {
                throw std::exception("Index out of range.");
            }

            m_selectedIndex = index;
            m_selectedValue = (index == -1) ? "" : m_possibleValues[index];

            if (this->m_SelectedIndexChangedEvent) {
                this->m_SelectedIndexChangedEvent(m_selectedIndex);
            }
        }

        /// @brief Sets the possible value.
        /// @param possibleValues A list of possible values that the combo-box can show
        /// @param selectedIndex Since we're updating the possible values, we need to set the
        ///        index of the item we're going to be showing after the update.
        ///        For no selecteion, -1 can be set.
        void SetPossibleValues(std::vector<std::string> possibleValues, int32_t selectedIndex){
            if ((selectedIndex > possibleValues.size() - 1) || (selectedIndex < -1)) {
                throw std::exception("SelectedIndex value out of range.");
            }

            this->m_selectedIndex = selectedIndex;
            this->m_possibleValues = possibleValues;
        }
    };
};

#endif // CCOMBOBOX_H
