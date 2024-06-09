#ifndef CARROWSRENDERER_H__
#define CARROWSRENDERER_H__

#include <QPainter>
#include "../Models/CArrowObject.h"

/// @brief Renderer in charge of drawing arrows serve to link
///        LMA to VMA where applicable
class CArrowsRenderer
{
public:
    /// @brief Renders the given arrow on the given painter
    void Render(const CArrowObject& tagretArrow, QPainter& painter);
};


#endif
