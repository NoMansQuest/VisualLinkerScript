#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <functional>

namespace VisualLinkerScript
{
    /// @brief Type widely used across the code-base
    template <typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    /// @brief Compares strings
    bool StringEquals(const std::string& a, const std::string& b, bool ignoreCase = false);    

    /// @brief Compares strings
    template <typename T>
    SharedPtrVector<T> LinqWhere(
            SharedPtrVector<T> source,
            std::function<bool(std::shared_ptr<T> element)> filterInput);

    /// @brief Compares strings
    template <typename TElement, typename TReturn>
    SharedPtrVector<TReturn> LinqSelect(
            SharedPtrVector<TElement> source,
            std::function<TReturn(std::shared_ptr<TElement> element)> transformFunction);

    /// @brief Compares strings
    template <typename TElement, typename TReturn>
    SharedPtrVector<TReturn> LinqDynamicPointerCast(
            SharedPtrVector<TElement> source);

    /// @brief Performs C++20 equivalent std::format.
    template<typename ... Args>
    std::string StringFormat( const std::string& format, Args ... args ) {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'

        if( size_s <= 0 ){
            throw std::runtime_error( "Error during formatting." );
        }

        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf( new char[ size ] );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }
}

#endif // HELPERS_H
