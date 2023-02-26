#ifndef CONSTANTS_H__
#define CONSTANTS_H__

#include <vector>
#include <string>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Class encapsulating all parser helper functions
    class CParserHelpers
    {
    public:
        /// @brief Is the word a reserved keyword, function name or command name?
        static bool IsReservedWord(const std::string& wordToCheck);

        /// @brief Is the word in question a Section-Output type?
        static bool IsSectionOutputType(const std::string& wordToCheck);

        /// @brief Is the word in question a potential function?
        static bool IsFunctionName(const std::string& wordToCheck);

        /// @brief Is the word in question a potential function in "Input Section"
        static bool IsInputSectionFunction(const std::string& wordToCheck);

        /// @brief Is the word in question a potential Assignment Procedure?
        static bool IsAssignmentProcedure(const std::string& wordToCheck);

        /// @brief Checks if the given operator is in fact an 'assignment' operator
        static bool IsAssignmentOperator(const std::string& operatorToCheck);

        /// @brief Checks if the given operator is in fact an 'assignment' operator
        static bool IsComparisonOperator(const std::string& operatorToCheck);

        /// @brief Checks if the given operator is a logical operator (e.g. "&&", "||", "==")
        static bool IsLogicalOperator(const std::string& operatorToCheck);

        /// @brief Checks if the given operator is an arithmetic operator (e.g. "+", "-", etc.)
        static bool IsArithmeticOperator(const std::string& operatorToCheck);

        /// @brief Checks if the given operator is a potential ternary operator (i.e. ":" or "?")
        static bool IsTernaryOperator(const std::string& operatorToCheck);

        /// @brief Checks if the given word is an illegal program-header name.
        static bool IsIllegalProgramHeaderName(const std::string& operatorToCheck);

        /// @brief Checks if the given word is valid program-header (PHDR) type
        static bool IsLegalProgramHeaderType(const std::string& operatorToCheck);

        /// @brief Checks if the given word is an Output-Section data function name.
        static bool IsOutputSectionDataFunctionName(const std::string& wordToCheck);

        /// @brief Checks if the given word is an Output-Section special function name (e.g. CREATE_OBJECT_SYMBOLS)
        static bool IsInputSectionSpecialFunctionName(const std::string& wordToCheck);
    };    
}

#endif // CONSTANTS_H
