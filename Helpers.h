#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>

namespace VisualLinkerScript
{
    /// @brief Compares strings
    bool StringEquals(const std::string& a, const std::string& b, bool ignoreCase = false);    

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
