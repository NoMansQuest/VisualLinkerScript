#ifndef CADDTEXTACTION_H__
#define CADDTEXTACTION_H__

#include "CActionBase.h"

namespace VisualLinkerScript::Models::Intervention
{
    /// @brief Describes text location and text to insert
    class CAddTextAction : public CActionBase
    {
    private:
        uint32_t m_startLocation;
        std::string m_textToAdd;

    public:
        /// @brief Default constructor
        CAddTextAction(uint32_t startLocation, std::string&& textToAdd)
            : CActionBase(startLocation),
              m_startLocation(startLocation),
              m_textToAdd(textToAdd)
        {}

        /// @brief Default desconstructor
        ~CAddTextAction() {}

    public:
        /// @brief Location of the first character to remove
        uint32_t StartLocation() {
            return m_startLocation;
        }

        /// @brief Text to insert at the "start location"
        const std::string& TextToAdd(){
            return m_textToAdd;
        }
    };
}

#endif // CADDTEXTACTION_H__
