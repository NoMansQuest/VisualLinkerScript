#ifndef SMETRIC_SIZEF_H__
#define SMETRIC_SIZEF_H__

#include "SPixelSizeF.h"
#include "../../../Helpers.h"

/// @brief Represents size in metric millimeters.
struct SMetricSizeF
{
    DECLARE_READONLY_PROPERTY(double, CX)
    DECLARE_READONLY_PROPERTY(double, CY)

    /// @brief Parameterized constructor
    SMetricSizeF(const double cx, const double cy) :
        m_CX(cx),
        m_CY(cy)
    {}

    /// @brief Default constructor
    SMetricSizeF() : SMetricSizeF(0, 0)
    {}

    /// @breif Converts size from millimeters to pixels
    SPixelSizeF ToPixelSize(const double dpiX, const double dpiY) const
    {
        return SPixelSizeF((this->CX() / 25.4) * dpiX, (this->CY() / 25.4) * dpiY);
    }
};

#endif // SMETRIC_SIZEF_H__
