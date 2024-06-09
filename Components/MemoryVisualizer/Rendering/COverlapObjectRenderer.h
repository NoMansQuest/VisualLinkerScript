#ifndef COVERLAPOBJECTRENDERER_H__
#define COVERLAPOBJECTRENDERER_H__

#include <QPainter>
#include "../Models/COverlayObject.h"

/// @brief Renderer in charge of drawing arrows serve to link
///        LMA to VMA where applicable
class CMemoryObjectRenderer
{
public:
    /// @brief Renders the given arrow on the given painter
    void Render(const COverlapObject& tagretMemoryObject, QPainter& painter);
};


#endif
