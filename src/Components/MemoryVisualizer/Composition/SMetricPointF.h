#ifndef SPOINTF_H__
#define SPOINTF_H__

#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Point-F used across the model
    struct SMetricPointF
    {
        DECLARE_READONLY_PROPERTY(double, X)
        DECLARE_READONLY_PROPERTY(double, Y)

        /// @brief Parameterized constructor
        SMetricPointF(const double x, const double y) :
            m_X(x),
            m_Y(y)
        {}

        /// @brief Default constructor
        SMetricPointF() : SMetricPointF(0, 0)
        {}

        /// @brief Derive an offset point.
        [[nodiscard]] SMetricPointF Offset(const double dx, const double dy) const
        {
            return { this->X() + dx, this->Y() + dy };
        }
    };
}

#endif // SPOINTF_H__
