#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <cstdint>

namespace VisualLinkerScript
{
    /// @brief Compares strings
    bool StringEquals(const std::string& a, const std::string& b, bool ignoreCase = false);
}


#endif // HELPERS_H
