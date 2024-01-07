#ifndef CPUBSUBEVENT_H
#define CPUBSUBEVENT_H

#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <memory>
#include <mutex>
#include <vector>

namespace VisualLinkerScript::Messaging
{
    namespace _internal
    {
        static uint64_t uniqueIdentifier = 1000000000L;
        static std::mutex uniqueIdentifierMutex;

        template <typename T>
        inline static constexpr size_t GetHashCode()
        {
            return typeid(T).hash_code();
        }
    }

    class CSubscriptionToken
    {
        public:
            inline CSubscriptionToken()
            {
                _internal::uniqueIdentifierMutex.lock();
                this->uuid_ = _internal::uniqueIdentifier++;
                _internal::uniqueIdentifierMutex.unlock();
            }

            inline ~CSubscriptionToken()
            {
                this->Unsubscribe();
            }

            inline void Unsubscribe()
            {
                if (deleter_ != nullptr)
                {
                    deleter_();
                }
            }

        private:
            uint64_t uuid_;
            std::function<void()> deleter_;
            template <typename> friend class PubSubEvent;
    };

    class CPubSubEventBase
    {
        public:
            virtual ~CPubSubEventBase() { }
    };

    template <typename TPayload>
    class CSubscription
    {
        public:
            inline CSubscription(const std::function<void(TPayload)>& callback)
            {
                callback_ = callback;
                token_ = std::make_shared<CSubscriptionToken>();
            }

        private:
            std::function<bool(TPayload)> filter_;
            std::function<void(TPayload)> callback_;
            std::shared_ptr<CSubscriptionToken> token_;

            template <typename> friend class PubSubEvent;
    };


    template <typename TPayload>
    class CPubSubEvent : public CPubSubEventBase
    {
        public:
            inline std::shared_ptr<CSubscriptionToken> Subscribe(const std::function<void(TPayload)>& callback, const std::function<bool(TPayload)>& filter = nullptr)
            {
                CSubscription<TPayload> new_subscription(callback);
                new_subscription.filter_ = filter;
                new_subscription.token_->deleter_ = [=]()
                {
                    /* Search for pair that contains the subscription uuid as the one we want to delete. */
                    auto it = std::remove_if(
                        subscriptions_.begin(),
                        subscriptions_.end(),
                        [=, this](CSubscription<TPayload> subscription)
                        {
                            return new_subscription.token_->uuid_ == subscription.token_->uuid_;
                        });

                    if (it != subscriptions_.end())
                    {
                        subscriptions_.erase(it);
                    }
                };

                subscriptions_.push_back(new_subscription);
                return new_subscription.token_;
            }

            inline void Publish(TPayload payload)
            {
                for (CSubscription<TPayload>& subscription : subscriptions_)
                {
                    if (subscription.filter_ != nullptr && !subscription.filter_(payload))
                    {
                        continue;
                    }

                    subscription.callback_(payload);
                }
            }

        private:
            std::vector<CSubscription<TPayload>> subscriptions_;
    };
}

#endif // CPUBSUBEVENT_H
