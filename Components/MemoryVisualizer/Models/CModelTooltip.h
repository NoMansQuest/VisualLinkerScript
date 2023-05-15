#ifndef CMODELTOOLTIP_H__
#define CMODELTOOLTIP_H__

#include <vector>
#include <string>
#include <cstdint>
#include "SPointF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Tool-Tip Object for the visualizer
    class CModelTooltip
    {
    public:
        /// @brief Default constructor
        CModelTooltip()
        {}

        /// @brief Destructor
        ~CModelTooltip()
        {}

    private:
        std::string m_toolTipContent = "";
        SPointF m_showingCoordinates;
        uint64_t m_appearanceTimestampInMilliseconds = 0;
        uint64_t m_displayDurationInMilliseconds = 0;
        bool m_isShowing = false;

    public:
        /// @brief Content of the tool-tip
        std::string ToolTipContent() {
            return this->m_toolTipContent;
        }

        /// @brief Reports back the display duration in milliseconds
        uint64_t DisplayDurationInMilliseconds() {
            return this->m_displayDurationInMilliseconds;
        }

        /// @brief Coordinates of the cursor where Tool-Tip began showing
        SPointF ShowingCoordinates() {
            return this->m_showingCoordinates;
        }

        /// @brief Returns back if the Tool-Tip is being shown
        bool IsShowing() {
            return this->m_isShowing;
        }

        /// @brief Returns the timestamp of the moment tool-tip started being displayed
        uint64_t AppearanceTimestampInMilliseconds() {
            return this->m_appearanceTimestampInMilliseconds;
        }

        /// @brief Sets the tool-tip content
        void SetToolTipContent(std::string newContent) {
            this->m_toolTipContent = newContent;
        }

        /// @brief Signals the system to start showing the tool-tip. 
        /// @param timestamp 
        void StartShowing(uint64_t timestampInMilliseconds, SPointF cursorCoordinates) {
            this->m_showingCoordinates = cursorCoordinates;
            this->m_appearanceTimestampInMilliseconds = timestampInMilliseconds;
        }
    };

}

#endif //
