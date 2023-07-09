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
    public:
        /// @brief Default constructor
        CMemoryObject(bool isExternal)
            : CSizableObject(false, isExternal)
        {}

    private:
        std::string m_title;
        SharedPtrVector<CContentBase> m_childContent;

    public:
        /// @brief Return title
        std::string Title(){
            return this->m_title;
        }

        /// @brief Upate title
        void SetTitle(std::string value){
            this->m_title = value;
        }

        /// @brief Returns back child content
        SharedPtrVector<CContentBase> ChildContent(){
            return this->m_childContent;
        }
    };
};

#endif // CMMEORYOBJECT_H__
