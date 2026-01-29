#ifndef CMASTER_PARSER_EXCEPTION_H__
#define CMASTER_PARSER_EXCEPTION_H__

#include <exception>
#include <string>

namespace VisualLinkerScript::ParsingEngine 
{
    /// @brief Each exception will have its own assigned enum entry
    enum class MasterParsingExceptionType
    {
        NotPresentEntryDetected,
        ParserMachineStateNotExpectedOrUnknown,
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
        const char* what() const noexcept override
        {
            return m_description.c_str();
        }

    public:
        /// @brief Detailed constructor
        explicit CMasterParsingException(
            MasterParsingExceptionType type,
            const std::string description)
        {
            this->m_description = description;
            this->m_type = type;            
        }

        /// @brief Default constructor
        explicit CMasterParsingException(
            MasterParsingExceptionType type)
        {
            this->m_description = "";
            this->m_type = type;
        }

        /// @brief Default destructor
        ~CMasterParsingException() {}

        /// @brief Type of exception
        MasterParsingExceptionType getExceptionType()
        {
            return m_type;
        }
    };
}


#endif
