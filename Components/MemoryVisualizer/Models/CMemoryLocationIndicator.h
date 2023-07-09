#ifndef CMEMORYLOCATIONINDICATOR_H__
#define CMEMORYLOCATIONINDICATOR_H__

#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents the "ENTRY()" directive.
    class CMemoryLocationIndicator : public CContentBase
    {
    // Member fields
    private:
        SPointF m_rightSidePosition;
        SPointF m_leftSidePosition;
        SRectangleF m_contentRectangle;


    public:

    };
};

#endif // CMEMORYLOCATIONINDICATOR_H__
