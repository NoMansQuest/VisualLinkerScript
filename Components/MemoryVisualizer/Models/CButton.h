#ifndef CBUTTON_H
#define CBUTTON_H

#include <vector>
#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Base class of all memory visualizer models
    class CButton : public CContentBase {
        DECLARE_STANDARD_PROPERTY(std::string, Text)
        DECLARE_READONLY_PROPERTY(std::function<void()>, PressedEvent);

    public:
        /// @brief Default Constructor;
        /// @param isDraggable Is the object dragabble?
        /// @param isExternal Is the object present as a result of an 'Include' directive?
        CButton(bool isDraggable, bool isExternal) : CContentBase(isDraggable, isExternal)
        {}
    };
}

#endif // CBUTTON_H
