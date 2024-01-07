#ifndef CNOTIFYUSEREVENT_H__
#define CNOTIFYUSEREVENT_H__

#include <string>

#include "../CPubSubEvent.h"

namespace VisualLinkerScript::Messaging::General
{
    /// @brief A simple text notification is to be shown to the user.
    class CNotifyUserEvent : public CPubSubEvent<CNotifyUserEvent>
    {
    private:
        std::string m_message;

    public:
        /// @brief Default constructor
        CNotifyUserEvent(std::string message)
            : m_message(message)
        {}

        /// @brief Reports back the message to be shown to the user.
        std::string Message() const
        {
            return this->m_message;
        }
    };
};

#endif
