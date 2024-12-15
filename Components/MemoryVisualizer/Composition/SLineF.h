#ifndef SLINE_H__
#define SLINE_H__

#include "../../../Helpers.h"
#include "SMetricPointF.h"
#include "CGraphicContext.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{

    /// @brief Line-F used across the model
    struct SLineF
    {
        DECLARE_READONLY_PROPERTY(double, StartX)
        DECLARE_READONLY_PROPERTY(double, StartY)
        DECLARE_READONLY_PROPERTY(double, EndX)
        DECLARE_READONLY_PROPERTY(double, EndY)

        /// @brief Returns the center point of the rectangle
        [[nodiscard]] SMetricPointF Center() const
        {
            return { (this->StartX() + this->EndX()) / 2.0f, (this->StartY() + this->EndY()) / 2.0f };
        }

        /// @brief Derive offset line
        [[nodiscard]] SLineF Offset(const double dx, const double dy) const
        {
            return {
                this->StartX() + dx,
                this->EndX() + dx,
                this->StartY() + dy,
                this->EndY() + dy
            };
        }

        /// @brief Default Constructor
        SLineF() : SLineF(0, 0, 0, 0)
        {}

        /// @brief Parameterized Constructor
        SLineF(const double startX, const double startY, const double endX, const double endY)
        {
            this->m_StartX = startX;
            this->m_StartY = startY;
            this->m_EndX = endX;
            this->m_EndY = endY;
        }

        /// @brief Get starting point in QPointF (in pixels, not metric anymore)
        [[nodiscard]] QPointF ToStartQPointF(const CGraphicContext& graphicContext) const
        {
            return { this->StartX() * graphicContext.DpmX(), this->StartY() * graphicContext.DpmY() };
        }

        /// @brief Get ending point in QPointF (in pixels, not metric anymore)
        [[nodiscard]] QPointF ToEndQPointF(const CGraphicContext& graphicContext) const
        {
            return { this->EndX() * graphicContext.DpmX(), this->EndY() * graphicContext.DpmY() };
        }
    };
}

#endif // SLINE_H__
