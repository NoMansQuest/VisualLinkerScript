#ifndef CPRELIMINARY_PARSING_EXCEPTION_H__
#define CPRELIMINARY_PARSING_EXCEPTION_H__

#include <exception>
#include <string>

namespace VisualLinkerScript { namespace ParsingEngine {

using namespace std;

/// @brief Each exception will have its own assigned enum entry
enum class PreliminaryParsingExceptionTypeEnum
{
    ContentWasNull, 
    InternalParserError,   
};


class CPreliminaryParsingException : public std::exception 
{

private:
    std::string m_description;
    PreliminaryParsingExceptionTypeEnum m_type;

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
        PreliminaryParsingExceptionTypeEnum type,
        const std::string description)
    {
        this->m_description = description;
        this->m_type = type;
    }

    ~CPreliminaryParsingException() {}


    /// @brief Type of exception
    PreliminaryParsingExceptionTypeEnum getExceptionType() const
    {
        return m_type;
    }
};

}}


#endif
