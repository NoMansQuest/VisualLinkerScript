#ifndef CPRELIMINARY_PARSING_EXCEPTION_H__
#define CPRELIMINARY_PARSING_EXCEPTION_H__

#include <exception>
#include <string>

namespace VisualLinkerScript { namespace ParsingEngine {

using namespace std;

/// @brief Each exception will have its own assigned enum entry
enum class PreliminaryParsingExceptionType
{
    ContentWasNull, 
    InternalParserError,   
};

/// @brief This class respresents an exception raised during preliminary parsing phase of the Linker-Script.
class CPreliminaryParsingException : public std::exception 
{

private:
    std::string m_description;
    PreliminaryParsingExceptionType m_type;

public:
    /// @brief The human-readable version of what went wrong
    const char* what() const
    {
        return m_description.c_str();
    }

public:
    /// @brief Default constructor
    /// @param exceptionDescription Text describing the issue (potentially displayed to the user) 
    explicit CPreliminaryParsingException(
        PreliminaryParsingExceptionType type,
        const std::string description)
    {
        this->m_description = description;
        this->m_type = type;
    }

    ~CPreliminaryParsingException() {}


    /// @brief Type of exception
    PreliminaryParsingExceptionType getExceptionType() const
    {
        return m_type;
    }
};

}}


#endif
