#ifndef SRECTANGLEF_H__
#define SRECTANGLEF_H__

#include "SPointF.h"

/// @brief Rectangle-F used across the model
struct SRectangleF
{
    DECLARE_READONLY_PROPERTY(double, Left)
    DECLARE_READONLY_PROPERTY(double, Top)
    DECLARE_READONLY_PROPERTY(double, Width)
    DECLARE_READONLY_PROPERTY(double, Height)

    /// @brief Default constructor;
    SRectangleF() : SRectangleF(0, 0, 0, 0)
    {}

    /// @brief Parameterized constructor;
    SRectangleF(
        const double left,
        const double top,
        const double width,
        const double height)
    {
        this->m_Height = height;
        this->m_Width = width;
        this->m_Left = left;
        this->m_Top = top;
    }

    /// @brief Returns the center point of the rectangle
    [[nodiscard]] SPointF Center() const
	{
        return SPointF{ this->Left() + (this->Width() / 2.0f), this->Top() + (this->Height() / 2.0f)};
    }

    /// @brief Derives an offset rectangle
    [[nodiscard]] SRectangleF Offset(const double dx, const double dy) const 
    {
        return { this->Left() + dx, this->Top() + dy, this->Width(), this->Height() };
    }
};    


#endif // SRECTANGLEF_H__
