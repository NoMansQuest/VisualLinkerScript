#ifndef SPIXEL_SIZEF_H__
#define SPIXEL_SIZEF_H__

#include "../../../Helpers.h"

/// @brief Represents size in pixels
struct SPixelSizeF
{
    DECLARE_READONLY_PROPERTY(double, CX)
    DECLARE_READONLY_PROPERTY(double, CY)

    /// @brief Parameterized constructor
    SPixelSizeF(const double cx, const double cy) :
        m_CX(cx),
        m_CY(cy)
    {}

    /// @brief Default constructor
    SPixelSizeF() : SPixelSizeF(0, 0)
    {}
};

#endif // SMETRIC_SIZEF_H__