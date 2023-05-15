#ifndef SPOINTF_H__
#define SPOINTF_H__

#include <vector>
#include <string>
#include <cstdint>

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Point-F used across the model
    struct SPointF {
        float x = 0.0f;
        float y = 0.0f;
    };
}

#endif // SPOINTF_H__
