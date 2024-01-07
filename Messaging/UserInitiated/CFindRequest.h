#ifndef CNOTIFYUSEREVENT_H__
#define CNOTIFYUSEREVENT_H__

#include <string>
#include "../CPubSubEvent.h"

namespace VisualLinkerScript::Messaging::UserInitiated
{
    using namespace VisualLinkerScript::Messaging;

    /// @brief Find scope
    enum class EFindRequestType
    {
        Selection,
        WholeText
    };

    /// @brief When a user requests to find text
    class CFindRequest : public CPubSubEvent<CFindRequest>
    {
    private:
        std::string m_contentToLookFor;
        bool m_isCaseSensitive;
        bool m_isRegularExpression;
        EFindRequestType m_findRequestType;

    public:
        CFindRequest(
                std::string contentToLookFor,
                bool isCaseSensitive,
                bool isRegularExpression,
                EFindRequestType findRequestType) :
            m_contentToLookFor(contentToLookFor),
            m_isCaseSensitive(isCaseSensitive),
            m_isRegularExpression(isRegularExpression),
            m_findRequestType(findRequestType)
        {}

    public:
        std::string ContentToLookFor() const
        {
            return this->m_contentToLookFor;
        }

        bool IsCaseSensitive() const
        {
            return this->m_isCaseSensitive;
        }

        bool IsRegularExpression() const
        {
            return this->m_isRegularExpression;
        }

        EFindRequestType FindRequestType() const
        {
            return this->m_findRequestType;
        }
    };
};





#endif
