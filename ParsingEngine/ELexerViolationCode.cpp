#include "ELexerViolationCode.h"
#include <stdexcept>

using namespace VisualLinkerScript::ParsingEngine;

std::string VisualLinkerScript::ParsingEngine::MapLexerViolationToDescription(ELexerViolationCode code)
{
    switch (code)
    {
    
    case ELexerViolationCode::UnexpectedCharacterDetected: return "Unexpected character detected";

    default:
        throw std::invalid_argument(NAMEOF(code));
    }
}


std::string VisualLinkerScript::ParsingEngine::MapLexerViolationToCode(ELexerViolationCode code)
{
    switch (code)
    {

    case ELexerViolationCode::UnexpectedCharacterDetected: return "L00001";

    default:
        throw std::invalid_argument(NAMEOF(code));
    }
}
