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

#endif // CMEMORYLOCATIONINDICATOR_H__
