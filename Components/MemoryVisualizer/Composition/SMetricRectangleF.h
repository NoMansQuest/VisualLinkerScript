#ifndef SRECTANGLEF_H__
#define SRECTANGLEF_H__

#include "../../Helpers.h"
#include "CGraphicContext.h"
#include "SMetricPointF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	using namespace VisualLinkerScript;

    /// @brief Rectangle-F used across the model
    struct SMetricRectangleF
    {
        DECLARE_READONLY_PROPERTY(double, Left)
        DECLARE_READONLY_PROPERTY(double, Top)
        DECLARE_READONLY_PROPERTY(double, Width)
        DECLARE_READONLY_PROPERTY(double, Height)
        DECLARE_READONLY_PROPERTY(double, Bottom)
        DECLARE_READONLY_PROPERTY(double, Right)

        /// @brief Default constructor;
        SMetricRectangleF() : SMetricRectangleF(0, 0, 0, 0)
        {}

        /// @brief Parameterized constructor;
        SMetricRectangleF(
            const double left,
            const double top,
            const double width,
            const double height)
        {
            this->m_Height = height;
            this->m_Width = width;
            this->m_Left = left;
            this->m_Top = top;
            this->m_Right = left + width;
            this->m_Bottom = top + height;
        }

        /// @brief Parameterized constructor, taking in QRect.
        SMetricRectangleF(
            const QRect rectangleInPixels,
            const double dpiX,
            const double dpiY)
        {
            this->m_Top = Graphical::GetMetricFromPixels(dpiY, rectangleInPixels.top());
            this->m_Left = Graphical::GetMetricFromPixels(dpiX, rectangleInPixels.left());
            this->m_Right = Graphical::GetMetricFromPixels(dpiX, rectangleInPixels.right());
            this->m_Height = Graphical::GetMetricFromPixels(dpiY, rectangleInPixels.height());
            this->m_Width = Graphical::GetMetricFromPixels(dpiY, rectangleInPixels.width());
        }

        /// @brief Returns the center point of the rectangle
        [[nodiscard]] SMetricPointF Center() const
        {
            return SMetricPointF{ this->Left() + (this->Width() / 2.0f), this->Top() + (this->Height() / 2.0f) };
        }

        /// @brief Derives an offset rectangle
        [[nodiscard]] SMetricRectangleF Offset(const double dx, const double dy) const
        {
            return { this->Left() + dx, this->Top() + dy, this->Width(), this->Height() };
        }

        /// @brief Converts this rectangle to a pixel-oriented QRect (intended for drawing)
        [[nodiscard]] QRectF ConvertToQRect(const CGraphicContext& graphicContext) const
        {
            auto scaledLeft = this->Left() * graphicContext.DpmX();
            auto scaledRight = this->Right() * graphicContext.DpmX();
            auto scaledTop = this->Top() * graphicContext.DpmY();
            auto scaledBottom = this->Bottom() * graphicContext.DpmY();

            return {scaledLeft, scaledTop, scaledRight - scaledLeft, scaledBottom - scaledTop};
        }
    };
}


#endif // SRECTANGLEF_H__
