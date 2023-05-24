#ifndef CLINKERSCRIPTOBJECT_H__
#define CLINKERSCRIPTOBJECT_H__

#include <vector>
#include "CContentBase.h"

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
              
        std::vector<CModelBase> m_children;

    public:
        /// @brief Memory objects declared within this Linker-Script
        const std::vector<CModelBase>& Children() {
            return this->m_children;
        }
    };
};

#endif // CLINKERSCRIPTOBJECT_H__
