#ifndef CLINKERSCRIPTOBJECT_H__
#define CLINKERSCRIPTOBJECT_H__

#include <vector>

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    class CMemoryObject;

    class CLinkerScriptObject
    {
    public:
        /// @brief Default Constructor;
        CLinkerScriptObject(uint64_t origin, uint64_t size, std::vector<CMemoryObject>&& memoryObjects)
            : m_origin(origin), m_size(size), m_memoryObjects(std::move(memoryObjects))
        {}

        /// @brief Destructor
        ~CLinkerScriptObject()
        {}

    // Member fields
    private:
        uint64_t m_origin;
        uint64_t m_size;
        std::vector<CMemoryObject> m_memoryObjects;

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

#endif // CLINKERSCRIPTOBJECT_H__