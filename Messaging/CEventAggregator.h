#ifndef CEVENTAGGREGATOR_H__
#define CEVENTAGGREGATOR_H__

#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <memory>
#include <mutex>
#include <vector>

#include "CPubSubEvent.h"

namespace VisualLinkerScript::Messaging
{
    /// @brief The event aggregator used across the project.
    class CEventAggregator
    {
        public:
            template <typename TEventType>
            inline std::shared_ptr<TEventType> GetEvent()
            {
                const size_t hash_code = _internal::GetHashCode<TEventType>();
                auto found = events_.find(hash_code);
                if (found == events_.end())
                {
                    auto event = std::make_shared<TEventType>();
                    events_[hash_code] = event;
                    return event;
                }

                return std::dynamic_pointer_cast<TEventType>(found->second);
            }

        private:
            std::unordered_map<size_t, std::shared_ptr<CPubSubEventBase>> events_;
    };

}

#endif
