#ifndef HELPERS_H__
#define HELPERS_H__

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <functional>
#include <qglobal.h>

#include "Components/QChromeTab/QChromeTabWidget.h"

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
        propType propName() const{\
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
        propType propName() const {\
            return m_##propName;\
        }

/// @brief Macro defines a virtual get/set property
#define DECLARE_VIRTUAL_PROPERTY( propType, propName ) \
    private:\
        propType m_##propName;\
                        \
    public:\
        virtual propType propName() const{\
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
    class LinqVector : public std::vector<std::shared_ptr<T>>
	{
    public:
        using BaseType = std::vector<std::shared_ptr<T>>;        
        using BaseType::BaseType;

        // Support for C# LINQ 'OfType' call
        template <typename U>
        LinqVector<U> OfType() const
    	{
            static_assert(std::is_base_of_v<T, U>, "U must be derived from T");
            LinqVector<U> result;
            for (const auto& obj : *this) 
            {
                if (auto casted = std::dynamic_pointer_cast<U>(obj)) 
                {
                    result.push_back(casted);
                }
            }
            return result;
        }

        // Support for C# LINQ 'Select' call (returning LinqVector of objects)
        template <typename Func>
        auto Select(Func&& func) const
            -> typename std::enable_if<
	            std::is_invocable_r<
		            std::shared_ptr<typename std::invoke_result<Func, std::shared_ptr<T>>::type::element_type>,
		            Func,
		            std::shared_ptr<T>
	            >::value,
	            LinqVector<typename std::invoke_result<Func, std::shared_ptr<T>>::type::element_type>
	            >::type
        {
            using ResultType = typename std::invoke_result<Func, std::shared_ptr<T>>::type;
            LinqVector<typename ResultType::element_type> result;

            for (const auto& obj : *this) {
                result.push_back(func(obj));
            }
            return result;
        }

        // Support for C# LINQ 'Select' call (returning std::vector of primitives)
        template <typename Func>
        auto Select(Func&& func) const
            -> typename std::enable_if<
	            std::is_invocable_r<bool, Func, std::shared_ptr<T>>::value,
	            std::vector<bool>
            >::type
        {
            std::vector<bool> result;
            for (const auto& obj : *this) {
                result.push_back(func(obj));
            }
            return result;
        }

        // Support for C# LINQ 'Where' call
        template <typename Func>
        LinqVector<T> Where(Func&& predicate) const {
            LinqVector<T> result;
            for (const auto& obj : *this) {
                if (predicate(obj)) {
                    result.push_back(obj);
                }
            }
            return result;
        }

        // Support for C# LINQ 'ForEach' call
        template <typename Func>
        void ForEach(Func&& action) const {
            for (const auto& obj : *this) {
                action(obj);
            }
        }

        // Support for C# LINQ 'SelectMany' call
        template <typename Func>
        auto SelectMany(Func&& func) const {
            using InnerVector = typename std::invoke_result<Func, std::shared_ptr<T>>::type;
            using InnerType = typename InnerVector::value_type::element_type;

            LinqVector<InnerType> result;
            for (const auto& obj : *this) {
                InnerVector inner = func(obj); // Apply the transformation
                result.insert(result.end(), inner.begin(), inner.end());
            }
            return result;
        }

        // Support for C# LINQ 'First' function
        std::shared_ptr<T> First() const
    	{
            if (this->empty()) {
                throw std::out_of_range("LinqPtrVector is empty.");
            }
            return this->front();
        }

        // Support for C# LINQ 'FirstOrDefault' function with optional default value
        std::shared_ptr<T> FirstOrDefault(std::shared_ptr<T> defaultValue = nullptr) const
    	{
            return this->empty() ? defaultValue : this->front();
        }

        // Support for C# LINQ 'Single' function
        std::shared_ptr<T> Single() const
    	{
            if (this->size() != 1) {
                throw std::logic_error("SharedPtrVector does not contain exactly one element.");
            }
            return this->front();
        }

        // Support for C# LINQ 'SingleOrDefault' function with optional default value
        std::shared_ptr<T> SingleOrDefault(std::shared_ptr<T> defaultValue = nullptr) const
    	{
            return (this->size() == 1) ? this->front() : defaultValue;
        }
    };

    /// @brief Merges 'vectorToAdd' into 'mainVector'
    template <typename T>
    void FuseVectors(std::vector<T>& mainVector, const std::vector<T>& vectorToAdd) {
        mainVector.insert(mainVector.end(), vectorToAdd.cbegin(), vectorToAdd.cend());
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

    namespace Graphical
    {
        /// @brief Self-explanatory .
        qreal GetFontSizeFromMetric(double dpiY, double desiredHeightMilliMeters);

		/// @brief Self-explanatory .
        qreal GetDpmFromDpi(double dpi);

        /// @brief Self-explanatory .
        qreal GetPixelsInMetric(double dpi, double desiredSizeInMilliMeters);

        /// @brief Self-explanatory .
        qreal GetMetricFromPixels(double dpi, double desiredSizeInPixels);

        /// @brief Returns the width of the text (single-lined).
        qreal GetTextWidthInPixels(const std::string& stringToMeasure, const QFontMetrics& fontMetrics);
    }

};

#endif // HELPERS_H__
