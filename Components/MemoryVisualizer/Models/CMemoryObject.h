#ifndef CMEMORYOBJECT_H__
#define CMEMORYOBJECT_H__

#include <vector>
#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Object.
    class CMemoryObject : public CContentBase
    {

    public:
        /// @brief Default constructor
        CMemoryObject() : CContentBase(true)
        {}

        /// @brief Default destructor
        ~CMemoryObject()
        {}

    private:


    public:
        std::vector<std::string> m_phdrList;
    };
};

#endif // CMMEORYOBJECT_H__
