#ifndef CSTATEMENTRENDERER_H__
#define CSTATEMENTRENDERER_H__

#include <QPainter>
#include "../Models/CStatementObject.h"

/// @brief Renderer in charge of drawing CStatementObject
class CStatementObjectRenderer
{
public:
    /// @brief Renders the given section-object
    void Render(const CStatementObject& tagretMemoryObject, QPainter& painter);
};

#endif
