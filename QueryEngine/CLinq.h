#ifndef CLINQ_H
#define CLINQ_H

#include "../Helpers.h"
#include <vector>
#include <memory>
#include <functional>

namespace VisualLinkerScript::QueryEngine
{
    using namespace VisualLinkerScript;

    /// @brief Provides C#-linke LINQ functionality
    template <typename TInput>
    class CLinq
    {
    private:
        std::vector<std::shared_ptr<TInput>> m_sourceVector;

    public:
        /// @brief Default constructor
        CLinq(std::vector<TInput>& sourceVector) :
            m_sourceVector(sourceVector)
        {}

        /// @brief Default constructor
        CLinq(std::vector<TInput>&& sourceVector) :
            m_sourceVector(std::move(sourceVector))
        {}

        template <typename TOutput>
        auto Select(std::function<TOutput(std::shared_ptr<TInput> inputElement)> selector) {
            std::vector<TOutput> result;
            for (auto element : m_sourceVector) {
                result.emplace_back(std::make_shared<TOutput>(selector(element)));
            }
            return CLinq<TOutput>(result);
        }

        template <typename TOutput>
        auto Where(std::function<bool(std::shared_ptr<TInput> inputElement)> selector) {
            std::vector<std::shared_ptr<TOutput>> result;
            for (TInput element : m_sourceVector) {
                if (!selector(element)) {
                    continue;
                }
                result.emplace_back(element);
            }
            return CLinq<TOutput>(result);
        }


        std::shared_ptr<TInput> FirstOrDefault() {
            if (m_sourceVector.empty()) {
                return nullptr;
            }
            return m_sourceVector[0];
        }

        std::shared_ptr<TInput> FirstOrDefault(std::function<bool(std::shared_ptr<TInput> inputElement)> predicate) {
            for (auto element : m_sourceVector) {
                if (predicate(element)) {
                    return element;
                }
            }
            return nullptr;
        }

        bool Any(std::function<bool(std::shared_ptr<TInput> inputElement)> predicate) {
            for (auto element : m_sourceVector) {
                if (predicate(element)) {
                    return true;
                }
            }
            return false;
        }

        bool All(std::function<bool(std::shared_ptr<TInput> inputElement)> predicate) {
            for (auto element : m_sourceVector) {
                if (!predicate(element)) {
                    return false;
                }
            }
            return true;
        }
    };
}

#endif // CLINQ_H
