#ifndef CSTATEMENTRENDERER_H__
#define CSTATEMENTRENDERER_H__

#include <QPainter>
#include "../Models/CTooltip.h"

/// @brief Renderer in charge of drawing CStatementObject
class CTooltipeRenderer
{
public:
    /// @brief Renders the given section-object
    void Render(const CTooltip& targetTooltipObject, QPainter& painter);
};

#endif
