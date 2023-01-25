#ifndef CONSTANTS_H__
#define CONSTANTS_H__

#include <vector>
#include <string>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    std::vector<std::string> ListOfProgramHeaderTypes = {
        "PT_NULL",
        "PT_LOAD",
        "PT_DYNAMIC",
        "PT_INTERP",
        "PT_NOTE",
        "PT_SHLIB",
        "PT_PHDR"
    };

    std::vector<std::string> ListOfIllegalProgramHeaderNames = {
        "PT_NULL",
        "PT_LOAD",
        "PT_DYNAMIC",
        "PT_INTERP",
        "PT_NOTE",
        "PT_SHLIB",
        "PT_PHDR",
        "FILEHDR",
        "PHDRS",
        "AT",
        "FLAGS",
    };

    std::vector<std::string> ListOfAssignmentCommandNames = {
        "PROVIDE",
        "PROVIDE_HIDDEN",
        "HIDDEN",
    };

    std::vector<std::string> ListOfFileRelatedCommandNames = {
        "INCLUDE",
        "INPUT",
        "GROUP",
        "AS_NEEDED",
        "OUTPUT",
        "SEARCH_DIR",
        "STARTUP",
    };

    std::vector<std::string> ListOfObjectRelatedCommandNames = {
        "OUTPUT_FORMAT",
        "TARGET",
    };

    std::vector<std::string> ListOfMemoryRelatedCommandNames = {
        "REGION_ALIAS",
    };

    std::vector<std::string> ListOfMiscellaneousCommandNames = {
        "ASSERT",
        "EXTERN",
        "FORCE_COMMON_ALLOCATION ",
        "INHIBIT_COMMON_ALLOCATION ",
        "FORCE_GROUP_ALLOCATION ",
        "INSERT BEFORE",
        "INSERT AFTER",
        "NOCROSSREFS",
        "NOCROSSREFS_TO",
        "OUTPUT_ARCH",
        "LD_FEATURE",
    };

    std::vector<std::string> ListOfFunctionNames = {
        "ENTRY",
        "ABSOLUTE",
        "ADDR",
        "ALIGN",
        "ALIGNOF",
        "BLOCK",
        "DATA_SEGMENT_ALIGN",
        "DATA_SEGMENT_END",
        "DATA_SEGMENT_RELRO_END",
        "DEFINED",
        "LENGTH",
        "LOADADDR",
        "LOG2CEIL",
        "MAX",
        "MIN",
        "NEXT",
        "ORIGIN",
        "SEGMENT_START",
        "SIZEOF",
        "SIZEOF_HEADERS",
    };

    std::vector<std::string> ListOfOutputSectionCommandNames = {
        "BYTE",
        "SHORT",
        "LONG",
        "QUAD",
        "SQUAD",
        "FILL",
    };

    std::vector<std::string> ListOfOutputSectionTypes = {
        "NOLOAD",
        "DSECT",
        "COPY",
        "INFO",
        "OVERLAY",
    };

    std::vector<std::string> ListOfReservedKeywords = {
        "PHDRS",
        "MEMORY",
        "SECTIONS",
        "VERSIONS",
        "EXCLUDE_FILE",
        "ORIGIN",
        "LENGTH",
    };    

    std::vector<std::string> ListOfLogicalOperators =
    {
        "==",
        "&&",
        "||",
        "!="
    };

    std::vector<std::string> ListOfTernaryOperators =
    {
        "?",
        ":"
    };

    std::vector<std::string> ListOfArithmeticOperators =
    {
        "+",
        "-",
        "*",
        "/",
        "%",
        "&",
        "|",
        "^"
    };

    std::vector<std::string> ListOfComparisonOperators =
    {
        "<",
        ">",
        "<=",
        ">=",
        "==",
        "!="
    };

    std::vector<std::string> ListOfAssignmentOperators =
    {
        ">>=",
        "<<=",
        "+=",
        "-=",
        "*=",
        "/=",
        "=",
        "&=",
        "|=",
        "%="
    };


    /// @brief Class encapsulating all parser helper functions
    class CParserHelpers
    {
    public:
        /// @brief Is the word a reserved keyword, function name or command name?
        static bool IsReservedWord(const std::string& wordToCheck)
        {
            auto concernedLists =
            {
                ListOfReservedKeywords,
                ListOfOutputSectionTypes,
                ListOfOutputSectionCommandNames,
                ListOfFunctionNames,
                ListOfMiscellaneousCommandNames,
                ListOfMemoryRelatedCommandNames,
                ListOfObjectRelatedCommandNames,
                ListOfFileRelatedCommandNames,
                ListOfAssignmentCommandNames
            };

            for (auto listToCheckAgainst: concernedLists)
            {
               if (std::find(listToCheckAgainst.cbegin(),
                             listToCheckAgainst.cend(),
                             wordToCheck ) != listToCheckAgainst.cend())
               {
                   return true;
               }
            }

            return false;
        }

        /// @brief Is the word in question a potential function?
        static bool IsFunctionName(const std::string& wordToCheck)
        {
            return std::find(ListOfFunctionNames.cbegin(),
                             ListOfFunctionNames.cend(),
                             wordToCheck) != ListOfFunctionNames.cend();
        }

        /// @brief Is the word in question a potential Assignment Procedure?
        static bool IsAssignmentProcedure(const std::string& wordToCheck)
        {
            return std::find(ListOfAssignmentCommandNames.cbegin(),
                             ListOfAssignmentCommandNames.cend(),
                             wordToCheck) != ListOfAssignmentCommandNames.cend();
        }

        /// @brief Checks if the given operator is in fact an 'assignment' operator
        static bool IsAssignmentOperator(const std::string& operatorToCheck)
        {
            return std::find(ListOfAssignmentOperators.cbegin(),
                             ListOfAssignmentOperators.cend(),
                             operatorToCheck) != ListOfAssignmentOperators.cend();
        }

        /// @brief Checks if the given operator is in fact an 'assignment' operator
        static bool IsComparisonOperator(const std::string& operatorToCheck)
        {
            return std::find(ListOfComparisonOperators.cbegin(),
                             ListOfComparisonOperators.cend(),
                             operatorToCheck) != ListOfComparisonOperators.cend();
        }

        /// @brief Checks if the given operator is a logical operator (e.g. "&&", "||", "==")
        static bool IsLogicalOperator(const std::string& operatorToCheck)
        {
            return std::find(ListOfLogicalOperators.cbegin(),
                             ListOfLogicalOperators.cend(),
                             operatorToCheck) != ListOfLogicalOperators.cend();
        }

        /// @brief Checks if the given operator is an arithmetic operator (e.g. "+", "-", etc.)
        static bool IsArithmeticOperator(const std::string& operatorToCheck)
        {
            return std::find(ListOfArithmeticOperators.cbegin(),
                             ListOfArithmeticOperators.cend(),
                             operatorToCheck) != ListOfArithmeticOperators.cend();
        }

        /// @brief Checks if the given operator is a potential ternary operator (i.e. ":" or "?")
        static bool IsTernaryOperator(const std::string& operatorToCheck)
        {
            return std::find(ListOfTernaryOperators.cbegin(),
                             ListOfTernaryOperators.cend(),
                             operatorToCheck) != ListOfTernaryOperators.cend();
        }
    };


}

#endif // CONSTANTS_H
