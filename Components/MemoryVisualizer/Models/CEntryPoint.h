#ifndef CENTRYPOINT_H__
#define CENTRYPOINT_H__

#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents the "ENTRY()" directive.
    class CEntryPoint : public CContentBase
    {
    public:
        /// @brief Default Constructor;
        CEntryPoint() : CContentBase(true)
        {}

        /// @brief Destructor
        ~CEntryPoint()
        {}

    // Member fields
    private:              
        std::string m_entryPointContent;

    public:
        /// @brief Memory objects declared within this Linker-Script
        const std::string& EntryPointContent()
        {
            return this->m_entryPointContent;
        }        

        /// @brief Updates the @see{m_entryPointContent}.
        void SetEntryPointContent(std::string newContent)
        {
            m_entryPointContent = newContent;
        }
    };
};

#endif // CMMEORYOBJECT_H__
