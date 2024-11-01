#ifndef HELPERS_H__
#define HELPERS_H__

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <functional>

/// @brief This does the "nameof" expression in c#
#define NAMEOF(x) #x

/// @brief We set here whether we're compiling for Unix-based or Windows
#if defined(Q_OS_LINUX) || defined(__APPLE__) || defined (__linux__) || defined(__unix__ )
    #define COMPILING_FOR_UNIX_BASED
#elif defined(Q_OS_WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__NT__)
    #define COMPILING_FOR_WINDOWS
#else
    #error Neither Q_OS_LINUX nor Q_OS_WIN32 were defined.
#endif

/// @brief Macro defines a standard get-set property
#define DECLARE_STANDARD_PROPERTY( propType, propName ) \
    private:\
        propType m_##propName;\
                        \
    public:\
        propType propName(){\
            return m_##propName;\
        }\
         \
        void Set##propName(propType value){\
            this->m_##propName = value;\
        }

/// @brief Macro defines a readonly get property
#define DECLARE_READONLY_PROPERTY( propType, propName ) \
    private:\
        propType m_##propName;\
                        \
    public:\
        propType propName(){\
            return m_##propName;\
        }

/// @brief Macro defines a virtual get/set property
#define DECLARE_VIRTUAL_PROPERTY( propType, propName ) \
    private:\
        propType m_##propName;\
                        \
    public:\
        virtual propType propName(){\
            return m_##propName;\
        }\
         \
        virtual void Set##propName(propType value){\
            this->m_##propName = value;\
        }


namespace VisualLinkerScript
{
    /// @brief Read back the content oa given file
    bool ReadFileContent(const std::string& filePath, std::string& fileContent, std::string& errorMessage);

    /// @brief Type widely used across the code-base
    template <typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    /// @brief Type widely used across the code-base
    template <typename T>
    using UniquePtrVector = std::vector<std::unique_ptr<T>>;

    /// @brief Merges 'vectorToAdd' into 'mainVector'
    template <typename T>
    void FuseVectors(std::vector<T>& mainVector, const std::vector<T>& vectorToAdd) {
        mainVector.insert(mainVector.end(), vectorToAdd.cbegin(), vectorToAdd.cend());
    }

    /// @brief Performs a 'Linq.Where()'
    template <typename T>
    SharedPtrVector<T> LinqWhere(
            const SharedPtrVector<T> source,
            std::function<bool(std::shared_ptr<T> element)> filterInput)
    {
        SharedPtrVector<T> returnList;
        for (const auto entry: source)
        {
             if (filterInput(entry))
             {
                 returnList.emplace_back(entry);
             }
        }
        return returnList;
    }

    /// @brief Performs a 'Linq.FirstOrDefault()'
    template <typename T>
    std::shared_ptr<T> LinqFirstOrDefault(
            const SharedPtrVector<T> source,
            std::function<bool(std::shared_ptr<T> element)> filterInput)
    {
        for (const auto entry: source)
        {
            if (filterInput(entry))
            {
                return entry;
            }
        }
        return nullptr;
    }

    /// @brief Performs a 'Linq.OfType<>'
    template <typename TBase, typename TReturn>
    SharedPtrVector<TReturn> LinqOfType(
            SharedPtrVector<TBase> source)
    {
        SharedPtrVector<TReturn> returnList;
        for (const auto entry: source)
        {
            auto converted = std::dynamic_pointer_cast<TReturn>(entry);
            if (converted != nullptr)
            {
                returnList.emplace_back(converted);
            }
        }
        return returnList;
    }

    /// @brief Performs a Linq.Select.
    template <typename TBase, typename TReturn>
    std::vector<TReturn> LinqSelect(
            const SharedPtrVector<TBase> source,
            std::function<TReturn(std::shared_ptr<TBase> element)> transformFunction)
    {
        std::vector<TReturn> returnList;
        for (const auto entry: source)
        {
            returnList.emplace_back(transformFunction(entry));
        }
        return returnList;
    }

    /// @brief Compares strings
    bool StringEquals(const std::string& a, const std::string& b, bool ignoreCase = false);

    /// @brief Searches for a given string in a vector to see if it is present.
    bool StringIn(const std::string& sourceString, const std::vector<std::string>& listToCheck, bool caseSensitive = true);

    /// @brief Performs L-Trim on a string and returns trimmed string.
    std::string StringLTrim(const std::string& sourceString);

    /// @brief Performs R-Trim on a string and returns trimmed string.
    std::string StringRTrim(const std::string& sourceString);

    /// @brief Performs left and right trim on a string and returns trimmed string.
    std::string StringTrim(const std::string& sourceString);

    /// @brief Splits a string based on a given character.
    std::vector<std::string> StringSplit(const std::string& sourceString, char seperator);

    /// @brief Performs C++20 equivalent std::format.
    template<typename ... Args>
    std::string StringFormat( const std::string& format, Args ... args )
    {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1;

        if( size_s <= 0 ){
            throw std::runtime_error( "Formatting error." );
        }

        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf( new char[ size ] );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 );
    }
};

#endif // HELPERS_H__
