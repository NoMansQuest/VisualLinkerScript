#ifndef CLINKERSCRIPTOBJECT_H__
#define CLINKERSCRIPTOBJECT_H__

#include <vector>
#include "CModelBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    class CLinkerScriptObject : public CModelBase
    {
    public:
        /// @brief Default Constructor;
        CLinkerScriptObject()
        {}

        /// @brief Destructor
        ~CLinkerScriptObject()
        {}

    // Member fields
    private:  
              
        std::vector<CMemoryObject> m_memoryObjects;

    public:
        /// @brief Memory objects declared within this Linker-Script
        const std::vector<CMemoryObject>& MemoryObjects()
        {
            return this->m_memoryObjects;
        }
    };
};

#endif // CLINKERSCRIPTOBJECT_H__
