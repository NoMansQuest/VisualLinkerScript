#ifndef CMEMORYOBJECTRENDERER_H__
#define CMEMORYOBJECTRENDERER_H__

#include <QPainter>
#include "../Models/CMemoryObject.h"

/// @brief Renderer in charge of drawing arrows serve to link
///        LMA to VMA where applicable
class CMemoryObjectRenderer
{
public:
    /// @brief Renders the given arrow on the given painter
    void Render(const CMemoryObject& tagretMemoryObject, QPainter& painter);
};


#endif
