#ifndef CSIZABLEOBJECT_H
#define CSIZABLEOBJECT_H

#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include "CMemoryLocationIndicator.h"
#include "CSizeIndicator.h"
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Object.
    class CSizableObject : public CContentBase
    {
    public:
        /// @brief Default constructor
        CSizableObject(bool isDraggable, bool isExternal)
            : CContentBase(isDraggable, isExternal)
        {}

    private:
        std::shared_ptr<CMemoryLocationIndicator> m_locationStart;
        std::shared_ptr<CMemoryLocationIndicator> m_locationEnd;
        std::shared_ptr<CSizeIndicator> m_sizeIndicator;

    public:
        std::vector<std::string> m_phdrList;
    };
};

#endif // CSIZABLEOBJECT_H
