#ifndef ELEXER_VIOLATION_CODE_H_
#define ELEXER_VIOLATION_CODE_H_

#include <cstdint>
#include <string>

namespace VisualLinkerScript::ParsingEngine
{
    /// @brief An exhaustive list of all known lexer violations.
    enum class ELexerViolationCode : uint32_t
    {
    	UnexpectedCharacterDetected // An unexpected character found in a given location
    };

    /// @brief Maps lexer violation code to string
    std::string MapLexerViolationToDescription(ELexerViolationCode code);

    /// @brief Maps lexer violation to unique ID code.
    std::string MapLexerViolationToCode(ELexerViolationCode code);
};

#endif //
