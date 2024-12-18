#include "Constants.h"
#include <vector>
#include <string>


namespace
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
        "OUTPUT_ARCH",
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
        "CONSTANT"
    };

    std::vector<std::string> ListOfOutputSectionCommandNames = {
        "BYTE",
        "SHORT",
        "LONG",
        "QUAD",
        "SQUAD",
        "FILL",
    };

    std::vector<std::string> ListOfInputSectionSpecialFunctionNames = {
        "CREATE_OBJECT_SYMBOLS"
    };

    std::vector<std::string> ListOfOutputSectionTypes = {
        "NOLOAD",
        "DSECT",
        "COPY",
        "INFO",
        "OVERLAY",
    };

    std::vector<std::string> ListOfScopeNames = {
        "PHDRS",
        "MEMORY",
        "SECTIONS",
        "VERSIONS",        
        "ORIGIN",
        "LENGTH",
    };

    std::vector<std::string> ListOfSectionOutputConstraints = {
        "ONLY_IF_RW",
        "ONLY_IF_RO",
    };

    std::vector<std::string> ListOfKeepFunctions = {
        "KEEP"
    };

    std::vector<std::string> ListOfExcludeListCall = {
        "EXCLUDE_FILE"
    };

    std::vector<std::string> ListOfSortFunctionCalls = {
        "SORT",
        "SORT_NONE",
        "SORT_BY_NAME",
        "SORT_BY_ALIGNMENT",
        "SORT_BY_INIT_PRIORITY"
    };

    std::vector<std::string> ListOfMemorySectionsReserveWords = {
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

    std::vector<std::string> ListOfWildcardStartEntries =
    {
        "?",
        "/",
        "*",
        "]",
        "[",
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

    std::string ToUpper(const std::string& source)
    {
        std::string convertionResult = source;
        for (auto & character: convertionResult)
        {
            character = toupper(character);
        }
        return convertionResult;
    }
};

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

bool CParserHelpers::IsReservedWord(const std::string& wordToCheck)
{
    auto concernedLists =
    {        
        ListOfScopeNames,
        ListOfInputSectionSpecialFunctionNames,
        ListOfExcludeListCall,
        ListOfSortFunctionCalls,
        ListOfMemorySectionsReserveWords,
        ListOfOutputSectionTypes,
        ListOfOutputSectionCommandNames,
        ListOfFunctionNames,
        ListOfMiscellaneousCommandNames,
        ListOfMemoryRelatedCommandNames,
        ListOfObjectRelatedCommandNames,
        ListOfFileRelatedCommandNames,
        ListOfAssignmentCommandNames
    };

    auto wordToCheckInUpperCase = ToUpper(wordToCheck);

    for (auto listToCheckAgainst: concernedLists)
    {
       if (std::find(listToCheckAgainst.cbegin(),
                     listToCheckAgainst.cend(),
                     wordToCheckInUpperCase) != listToCheckAgainst.cend())
       {
           return true;
       }
    }

    return false;
}

bool CParserHelpers::IsOutputSectionDataFunctionName(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfOutputSectionCommandNames.cbegin(),
                     ListOfOutputSectionCommandNames.cend(),
                     wordToCheckInUpperCase) != ListOfOutputSectionCommandNames.cend();
}

bool CParserHelpers::IsInputSectionSpecialFunctionName(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfInputSectionSpecialFunctionNames.cbegin(),
                     ListOfInputSectionSpecialFunctionNames.cend(),
                     wordToCheckInUpperCase) != ListOfInputSectionSpecialFunctionNames.cend();
}

bool CParserHelpers::IsSortFunction(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfSortFunctionCalls.cbegin(),
                     ListOfSortFunctionCalls.cend(),
                     wordToCheckInUpperCase) != ListOfSortFunctionCalls.cend();
}

bool CParserHelpers::IsExcludeFileCall(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfExcludeListCall.cbegin(),
                     ListOfExcludeListCall.cend(),
                     wordToCheckInUpperCase) != ListOfExcludeListCall.cend();
}

bool CParserHelpers::IsIllegalProgramHeaderName(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfIllegalProgramHeaderNames.cbegin(),
                     ListOfIllegalProgramHeaderNames.cend(),
                     wordToCheckInUpperCase) != ListOfIllegalProgramHeaderNames.cend();
}

bool CParserHelpers::IsLegalProgramHeaderType(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfProgramHeaderTypes.cbegin(),
                     ListOfProgramHeaderTypes.cend(),
                     wordToCheckInUpperCase) != ListOfProgramHeaderTypes.cend();
}

bool CParserHelpers::IsSectionOutputType(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfOutputSectionTypes.cbegin(),
                     ListOfOutputSectionTypes.cend(),
                     wordToCheckInUpperCase) != ListOfOutputSectionTypes.cend();
}

bool CParserHelpers::IsFunctionName(const std::string& wordToCheck)
{
    auto concernedLists =
    {
        ListOfFunctionNames,
        ListOfFileRelatedCommandNames,
        ListOfObjectRelatedCommandNames
    };

    auto wordToCheckInUpperCase = ToUpper(wordToCheck);

    for (auto listToCheckAgainst: concernedLists)
    {
       if (std::find(listToCheckAgainst.cbegin(),
                     listToCheckAgainst.cend(),
                     wordToCheckInUpperCase) != listToCheckAgainst.cend())
       {
           return true;
       }
    }

    return false;
}

bool CParserHelpers::IsKeepFunction(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfKeepFunctions.cbegin(),
                     ListOfKeepFunctions.cend(),
                     wordToCheckInUpperCase) != ListOfKeepFunctions.cend();
}


bool CParserHelpers::IsAssignmentProcedure(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfAssignmentCommandNames.cbegin(),
                     ListOfAssignmentCommandNames.cend(),
                     wordToCheckInUpperCase) != ListOfAssignmentCommandNames.cend();
}

bool CParserHelpers::IsSectionOutputConstraint(const std::string& wordToCheck)
{
    auto wordToCheckInUpperCase = ToUpper(wordToCheck);
    return std::find(ListOfSectionOutputConstraints.cbegin(),
                     ListOfSectionOutputConstraints.cend(),
                     wordToCheckInUpperCase) != ListOfSectionOutputConstraints.cend();
}

bool CParserHelpers::IsAssignmentOperator(const std::string& operatorToCheck)
{
    return std::find(ListOfAssignmentOperators.cbegin(),
                     ListOfAssignmentOperators.cend(),
                     operatorToCheck) != ListOfAssignmentOperators.cend();
}

bool CParserHelpers::IsComparisonOperator(const std::string& operatorToCheck)
{
    return std::find(ListOfComparisonOperators.cbegin(),
                     ListOfComparisonOperators.cend(),
                     operatorToCheck) != ListOfComparisonOperators.cend();
}

bool CParserHelpers::IsLogicalOperator(const std::string& operatorToCheck)
{
    return std::find(ListOfLogicalOperators.cbegin(),
                     ListOfLogicalOperators.cend(),
                     operatorToCheck) != ListOfLogicalOperators.cend();
}

bool CParserHelpers::IsArithmeticOperator(const std::string& operatorToCheck)
{
    return std::find(ListOfArithmeticOperators.cbegin(),
                     ListOfArithmeticOperators.cend(),
                     operatorToCheck) != ListOfArithmeticOperators.cend();
}

bool CParserHelpers::IsStartOfWildcard(const std::string& wordToCheck)
{
    return std::find(ListOfWildcardStartEntries.cbegin(),
                     ListOfWildcardStartEntries.cend(),
                     wordToCheck) != ListOfWildcardStartEntries.cend();
}

bool CParserHelpers::IsSemicolon(const std::string& operatorToCheck)
{
    return operatorToCheck == ";";
}

bool CParserHelpers::IsColon(const std::string& operatorToCheck)
{
    return operatorToCheck == ":";
}

bool CParserHelpers::IsComma(const std::string& operatorToCheck)
{
    return operatorToCheck == ",";
}

bool CParserHelpers::IsTernaryOperator(const std::string& operatorToCheck)
{
    return std::find(ListOfTernaryOperators.cbegin(),
                     ListOfTernaryOperators.cend(),
                     operatorToCheck) != ListOfTernaryOperators.cend();
}
