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
};


class CPreliminaryParsingException : public std::exception 
{

private:
    std::string m_exceptionSource;
    PreliminaryParsingExceptionTypeEnum m_exceptionType;

public:
    /// @brief The human-readable version of what went wrong
    const char* what() const
    {
        return m_exceptionSource.c_str();
    }

public:
    /// @brief Default constructor
    /// @param exceptionDescription Text describing the issue (potentially displayed to the user) 
    explicit CPreliminaryParsingException(
        PreliminaryParsingExceptionTypeEnum exceptionType, 
        const std::string exceptionDescription)
    {
        this->m_exceptionSource = exceptionDescription;
    }

    ~CPreliminaryParsingException() {}


    /// @brief Type of exception
    PreliminaryParsingExceptionTypeEnum getExceptionType()
    {
        return m_exceptionType;
    }
};

}}


#endif
