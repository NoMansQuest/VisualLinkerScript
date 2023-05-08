#ifndef CMODELBASE_H
#define CMODELBASE_H

#include <vector>

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    class CModelBase
    {
    public:
        /// @brief Default Constructor;
        CModelBase(uint64_t origin, uint64_t size, std::vector<CMemoryObject>&& memoryObjects)
            : m_origin(origin), m_size(size), m_memoryObjects(std::move(memoryObjects))
        {}

        /// @brief Destructor
        ~CLinkerScriptObject()
        {}

    // Member fields
    private:
        bool m_isHoveringOver;
        bool m_isSelected;

        float m_xPos;
        float m_yPos;
        float m_width;
        float m_height;


    public:
        /// @brief Origin of the memory object
        uint64_t Origin() { return this->m_origin; }

        /// @brief Size of the memory object
        uint64_t Size() { return this->m_size; }

        /// @brief Memory objects declared within this Linker-Script
        const std::vector<CMemoryObject>& MemoryObjects()
        {
            return this->m_memoryObjects;
        }

    };
};

#endif // CMODELBASE_H
