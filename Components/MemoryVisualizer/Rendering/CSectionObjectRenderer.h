#ifndef CSECTIONOBJECTRENDERER_H__
#define CSECTIONOBJECTRENDERER_H__

#include <QPainter>
#include "../Models/CSectionObject.h"

/// @brief Renderer in charge of drawing CSectionObject inside CMemoryObject
class CSectionObjectRenderer
{
public:
    /// @brief Renders the given section-object
    void Render(const CSectionObject& tagretMemoryObject, QPainter& painter);
};

#endif
