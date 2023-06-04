#ifndef CFLOORPLAN_H__
#define CFLOORPLAN_H__

#include <vector>
#include "CContentBase.h"
#include "CLmaToVmaArrow.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Linker-Script object.
    class CFloorplan
    {
    // Member fields
    private:                
        std::vector<CContentBase> m_content;
        std::vector<CLmaToVmaArrow> m_arrows;

    public:
        /// @brief Memory content
        const std::vector<CContentBase>& Content() {
            return this->m_content;
        }

        /// @brief Arrows
        const std::vector<CLmaToVmaArrow>& Arrows() {
            return this->m_arrows;
        }
    };
};

#endif // CFLOORPLAN_H__
