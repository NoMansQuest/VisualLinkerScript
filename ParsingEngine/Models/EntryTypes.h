#ifndef ENTRY_TYPES_H__
#define ENTRY_TYPES_H__

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {
{    
    /// @brief Type of entries procued by Preliminary-Parsing engine
    enum class EntryTypesEnum
    {
        /// @brief A word, which may only be a single dot as well
        Word,   

        /// @brief Operators include '*', '=', '+', '-', '>', '<', ';', '/', etc.
        Operator,

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
    }
}}}

#endif