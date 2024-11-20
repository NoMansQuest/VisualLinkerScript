#ifndef SPOINTF_H__
#define SPOINTF_H__

/// @brief Point-F used across the model
struct SPointF
{
    DECLARE_READONLY_PROPERTY(double, X)
    DECLARE_READONLY_PROPERTY(double, Y)

    /// @brief Parameterized constructor
	SPointF(const double x, const double y) :
		m_X(x),
		m_Y(y)
    {}

    /// @brief Default constructor
    SPointF() : SPointF(0,0)
    {}
    
    /// @brief Derive an offset point.
    [[nodiscard]] SPointF Offset(const double dx, const double dy) const
    {
        return { this->X() + dx, this->Y() + dy };
    }
};

#endif // SPOINTF_H__
