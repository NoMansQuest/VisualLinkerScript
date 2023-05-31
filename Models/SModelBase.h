#ifndef SMODELBASE_H
#define SMODELBASE_H

#include <string>
#include <cstdlib>

namespace VisualLinkerScript::Models
{
    /// @brief Every model object has a type
    enum class EModelType
    {
        Root,       // This would be the linker-script itself
        Comment,    // This represents the 'Comment' object
    };

    /// @brief Base object for every element in the Model
    struct SModelBase
    {
        /// @brief Index of the character where this content starts at
        ///        in the linker-script
        size_t StartOffset;

        /// @brief Index of the last character of this object
        size_t EndOffset;

        /// @brief Helper function returning the length of the object
        size_t Length() { return this->EndOffset - this->StartOffset; }

        /// @brief Full path to this object
        std::string ObjectPath;

        /// @brief Is the object valid? If not, it means it contains errors.
        bool IsValid;

        /// @brief Type of the object, must be defined by each type.
        virtual EModelType Type() = 0;
    };
};

#endif // SMODELBASE_H
