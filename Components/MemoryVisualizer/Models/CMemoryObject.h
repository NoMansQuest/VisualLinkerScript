#ifndef CMEMORYOBJECT_H__
#define CMEMORYOBJECT_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CSizableObject.h"
#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Object.
    class CMemoryObject : public CSizableObject
    {
        DECLARE_STANDARD_PROPERTY( std::string, Title )
        DECLARE_READONLY_PROPERTY( SharedPtrVector<CContentBase>, ChildContent )

    public:
        /// @brief Default constructor
        CMemoryObject(bool isExternal)
            : CSizableObject(false, isExternal)
        {}
    };
};

#endif // CMMEORYOBJECT_H__
