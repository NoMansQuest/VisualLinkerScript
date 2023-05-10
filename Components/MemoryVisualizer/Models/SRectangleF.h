#ifndef SRECTANGLEF_H__
#define SRECTANGLEF_H__

#include <vector>
#include <string>
#include <cstdint>
#include "SPointF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Rectangle-F used across the model
    struct SRectangleF
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Width = 0.0f;
        float Height = 0.0f;

        /// @brief Returns the 'Left' position of the rectangle
        float Left() { return this->X; }

        /// @brief Returns the 'Right' position of the rectangle 
        float Right() { return this->Y + this->Width; }

        /// @brief Returns the 'Top' position of the rectangle
        float Top() { return this->Y; }

        /// @brief Reports the 'Bottom' position of the rectangle
        float Bottom() { return this->Y + this->Height; }

        /// @brief Returns the center point of the rectangel
        SPointF Center() 
        { 
            return SPointF{this->X + (this->Width / 2.0f), this->Y + (this->Height / 2.0f)}
        }
    };    
}

#endif // SRECTANGLEF_H__