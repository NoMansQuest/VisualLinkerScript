#ifndef CREMOVETEXTACTION_H__
#define CREMOVETEXTACTION_H__

#include "CActionBase.h"
#include <cstdint>

namespace VisualLinkerScript::Models::Intervention
{
    /// @brief Describes the text to be removed
    class CRemoveTextAction : public CActionBase
    {
    private:
        uint32_t m_startLocation;
        uint32_t m_length;

    public:
        /// @brief Default constructor
        CRemoveTextAction(uint32_t startLocation, uint32_t length)
            : CActionBase(startLocation),
              m_startLocation(startLocation),
              m_length(length)
        {}

        /// @brief Default desconstructor
        ~CRemoveTextAction() {}

    public:
        /// @brief Location of the first character to remove
        uint32_t StartLocation() {
            return m_startLocation;
        }

        /// @brief Length of the text to remove.
        uint32_t Length(){
            return m_length;
        }
    };
}

#endif // CREMOVETEXTACTION_H__
