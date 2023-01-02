#ifndef CMASTER_PARSER_EXCEPTION_H__
#define CMASTER_PARSER_EXCEPTION_H__

#include <exception>
#include <string>

using namespace std;

namespace VisualLinkerScript::ParsingEngine 
{
    /// @brief Each exception will have its own assigned enum entry
    enum class MasterParsingExceptionType
    {
        NotPresentEntryDetected,
        UnrecognizableRawEntryTypeValueFound,
        NoParserWasAbleToProceed,
        InternalParserError,   
    };

    /// @brief This class respresents an exception raised during master parsing phase of the Linker-Script.
    class CMasterParsingException : public std::exception 
    {

    private:
        std::string m_description;
        MasterParsingExceptionType m_type;

    public:
        /// @brief The human-readable version of what went wrong
        const char* what() const
        {
            return m_description.c_str();
        }

    public:
        /// @brief Default constructor
        explicit CPreliminaryParsingException(
            MasterParsingExceptionType type,
            const std::string description)
        {
            this->m_description = description;
            this->m_type = type;
        }

        /// @brief Default constructor
        explicit CPreliminaryParsingException(
            MasterParsingExceptionType type)
        {
            this->m_description = "";
            this->m_type = type;
        }

        ~CPreliminaryParsingException() {}


        /// @brief Type of exception
        MasterParsingExceptionType getExceptionType() const
        {
            return m_type;
        }
    };
}


#endif
