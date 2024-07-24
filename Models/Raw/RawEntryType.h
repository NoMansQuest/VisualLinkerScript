#ifndef ENTRY_TYPES_H__
#define ENTRY_TYPES_H__

#include <cstdint>

namespace VisualLinkerScript::Models::Raw 
{
    /// @brief Type of entries extracted by the lexer
    enum class RawEntryType : uint32_t
    {
        /// @brief This element is not present, and serves as a place-holder only.
        NotPresent,

        /// @brief A word, which may only be a single dot as well
        Word,   

        /// @brief Included: >, <, >=, <=, ==, !=, ||, &&
        EvaluativeOperators,

        /// @brief Operators include '*', '+', '-', '/', '%', '&', '|', '^'
        ArithmeticOperator,

        /// @brief Assignment operators: '=', '*=', '+=', '-=', '/=', '>>=' , '<<=', '|=', '&=', "~=", '^='
        AssignmentOperator,

        /// @brief Include '[', ']', '\\'. Other wildcard characters are (i.e. '*', '-', '?') are subject to interpretation.
        Wildcard,

        /// @brief Comma: ','
        Comma,

        /// @brief Semicolon: ';'
        Semicolon,

        /// @brief Colon: ':'
        Colon,

        /// @brief Question Mark: "?"
        QuestionMark,

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
