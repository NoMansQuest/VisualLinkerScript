#ifndef ENTRY_TYPES_H__
#define ENTRY_TYPES_H__

#include <cstdint>

namespace VisualLinkerScript::ParsingEngine::Models::Raw 
{
    /// @brief Type of entries procued by Preliminary-Parsing engine
    enum class RawEntryType : uint32_t
    {
        /// @brief This element is not present, and serves as a placedholder only.
        NotPresent,

        /// @brief A word, which may only be a single dot as well
        Word,   

        /// @brief Operators include '*', '+', '-', '>', '<', ';', '/', '?', '%'etc.
        Operator,

        /// @brief Assignment operators: '=', '*=', '+=', '-=', '/=', '>>=' , '<<=', '|=', '&='
        Assignment,

        /// @brief A number. This number may or may not be valid
        Number,

        /// @brief Strings may also be present, although definitely counted as bad coding practice.
        String,

        /// @brief Comments may also exist
        Comment,

        /// @brief Represents '('
        ParenthesisOpen,

        /// @brief Represents ')'
        ParenthesisClose,

        /// @brief Represents '{'
        BracketOpen,

        /// @brief Represents '}'
        BracketClose,        

        /// @brief Unknown type of operator. For example a backslash may be one
        Unknown        
    };
}

#endif
