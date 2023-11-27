#include <memory>
#include <string>
#include <vector>

#include "CLexer.h"
#include "CLexerException.h"

#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Helpers.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine;

namespace
{
    /// @brief Various states of the parser.
    enum class ParserStates
    {
        Default,
        InComment,        
        InString,
        InWord,
        InNumber,
    };

    /// @brief Types of assignment symbols we can have in a linker-script (i.e. =, +=, -=, &=, >>=, etc.)
    enum class AssignmentSymbolTypes : uint32_t
    {
        SingleCharacter = 1,
        DoubleCharacter = 2,
        TripleCharacter = 3,
        NotAnAssignmentSymbol = 4
    };

    /// @brief Types of evaluative symbols we can have in a linker-script (i.e. >=, <=, ==, etc.)
    enum class EvaluativeSymbolTypes : uint32_t
    {
        SingleCharacter = 1,
        DoubleCharacter = 2,
        NotAnEvaluativeSymbol = 3
    };

    /// @brief The loop needs alignment after an entry is detected and registerd
    enum class ParserLoopActionType
    {
        StartFromSamePosition,
        StartFromOneCharacterAfter,
        StartFromOneCharacterBefore,
        StartFromTwoCharactersAfter
    };

    /// @brief Checks if the character at the given position matches @see testAgainst.
    bool SafeTestCharacterInString(const std::string& input, uint32_t position, char testAgainst)
    {   
        return (position <= (input.length() - 1)) && (input[position] == testAgainst);
    }

    /// @brief Checks for the pattern of two characters at given positions. This is to optimize performance.
    bool SafeTestPatternInString(
        const std::string& input,
        uint32_t position, 
        const char firstCharacter,
        const char secondCharacter)
    {
        return SafeTestCharacterInString(input, position, firstCharacter) &&
               SafeTestCharacterInString(input, position + 1, secondCharacter);
    }

    /// @brief Determines if this character can be the first letter of a word
    bool CanCharBeStartOfWord(const char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||
               (input == '_')  || (input == '.');     // Wildcard support, could also be an operator
    }

    /// @brief If true, then the character can be considered as part of a word
    bool CanCharBeWithinWord(char input)
    {
        return ((input >= '0') && (input <= '9')) || CanCharBeStartOfWord(input);
    }

    /// @brief Checks to see if a given character can be an arithmetic operator.
    ///        These include right-shift and left-shift operators ('<<' and '>>').
    bool IsArithmeticOperator(const char input, const char inputPlusOne, uint32_t& arithmeticOperatorLength)
    {
        arithmeticOperatorLength = 1;

        if ((input == '>' && inputPlusOne == '>') ||
            (input == '<' && inputPlusOne == '<'))
        {
            arithmeticOperatorLength = 2;
            return true;
        }

        return ((input == '*') || (input == '>') ||  (input == '<') || (input == '/') ||
                (input == '-') || (input == '+') ||  (input == '&') || (input == '|') ||
                (input == '%') || (input == '^') ||  (input == '~') || (input == '!'));
    }

    /// @brief Checks to see if the character is a semicolon.
    bool IsSemicolon(const char input)
    {
        return input == ';';
    }

    /// @brief Checks to see if the character is a colon.
    bool IsColon(const char input)
    {
        return input == ':';
    }

    /// @brief Checks to see if the character is a question mark.
    bool IsQuestionMark(const char input)
    {
        return input == '?';
    }

    /// @brief Checks to see if character is a comma.
    bool IsComma(const char input)
    {
        return input == ',';
    }

    /// @brief Checks the character for being a potential prefix (such as 0x, 0b, etc.)
    bool IsNumberPrefix(char input)
    {
        return ((input == 'x') || (input == 'X') || (input == 'b') || (input == 'B'));
    }

    /// @brief Checks the character for being a potential postfixes ('K' and 'M')
    bool IsNumberPostfix(char input)
    {
        return ((input == 'K') ||
                (input == 'M'));
    }

    /// @brief The following characters are considered wild-card characters. The list here is
    ///        not exchaustive (*, ? and - can also be wildcards), however the other character's 
    ///        role as wild-card as subjective. 
    /// @param input 
    /// @return 
    bool IsWildCardCharacter(char input)
    {
        return (input == '[') ||
               (input == ']') ||
               (input == '\\');               
    }

    /// @brief Detects if the input character is a white-space
    bool IsWhitespace(char input)
    {
        return ((input >= (char)0x09) && (input <= (char)0x0D)) || (input == ' ');
    }

    /// @brief Checks if input is a digit. 
    bool IsNumber(char input)
    {
        return (input >= '0') && (input <= '9');
    }


    /// @brief Checks if input is a digit or a hexadecimal. 
    bool IsNumberOrHex(char input)
    {
        return ((input >= '0') && (input <= '9')) || 
               ((input >= 'A') && (input <= 'F')) ||
               ((input >= 'a') && (input <= 'f'));
    }

    /// @brief Checks if input is a double-quotation symbo, marking the start/end of a string
    bool IsDoubleQuotation(char input)
    {
        return (input == '"');
    }

    /// @brief Checks if input is Line-Feed symbol
    bool IsLineFeed(char input)
    {
        return (input == '\n');
    }

    /// @brief Checks to see if assignment symbol is present, 
    ///        such as '/=', '*=', '+=', '-=', '=', '|=', '&=', '^=', '%='
    AssignmentSymbolTypes TestForAssignmentSymbol(const std::string& input, const uint32_t position)
    {
        auto charAtPosition = input[position];
        if (SafeTestCharacterInString(input, position, '='))
        {
            return (!SafeTestCharacterInString(input, position + 1, '=')) ?
                    AssignmentSymbolTypes::SingleCharacter :
                    AssignmentSymbolTypes::NotAnAssignmentSymbol; // Note: '==' is not an assignment operator, but an evaluation operator.
        }
        else if ((charAtPosition == '*') || (charAtPosition == '/') || (charAtPosition == '%') || (charAtPosition == '-') ||
                 (charAtPosition == '~') || (charAtPosition == '|') || (charAtPosition == '&') || (charAtPosition == '^') ||
                 (charAtPosition == '+'))
        {
            return (!SafeTestCharacterInString(input, position + 1, '=')) ?
                   AssignmentSymbolTypes::NotAnAssignmentSymbol :
                   AssignmentSymbolTypes::SingleCharacter;
        }
        else if ((charAtPosition == '>') || (charAtPosition == '<'))
        {
            bool isAssignment = (SafeTestCharacterInString(input, position + 1, charAtPosition) &&
                                 SafeTestCharacterInString(input, position + 2, '='));

            return isAssignment ? AssignmentSymbolTypes::TripleCharacter : AssignmentSymbolTypes::NotAnAssignmentSymbol;
        }

        return AssignmentSymbolTypes::NotAnAssignmentSymbol;
    }

        /// @brief Test against evaluative operators:
    ///        such as '<=', '>=', '==', '!=', '<', '>', '||', '&&'
    EvaluativeSymbolTypes TestForEvaluativeSymbol(const std::string& input, const uint32_t position)
    {
        auto charAtPosition = input[position];

        if (charAtPosition == '>' || charAtPosition == '<')
        {
            if (SafeTestCharacterInString(input, position + 1, charAtPosition))
            {
                if (SafeTestCharacterInString(input, position + 2, '='))
                {
                    return EvaluativeSymbolTypes::NotAnEvaluativeSymbol; // This is an assignment operator
                }
            }
            else if (SafeTestCharacterInString(input, position + 1, '='))
            {
                return EvaluativeSymbolTypes::DoubleCharacter;
            }
            else
            {
                 return EvaluativeSymbolTypes::SingleCharacter;
            }
        }
        else if (charAtPosition == '=')
        {
            return (SafeTestCharacterInString(input, position + 1, '=')) ?
                   EvaluativeSymbolTypes::DoubleCharacter :
                   EvaluativeSymbolTypes::NotAnEvaluativeSymbol;
        }
        else if (charAtPosition == '!')
        {
            return SafeTestCharacterInString(input, position + 1, '=') ?
                   EvaluativeSymbolTypes::SingleCharacter :
                   EvaluativeSymbolTypes::NotAnEvaluativeSymbol ;
        }
        else if ((charAtPosition == '|' || charAtPosition == '&') && SafeTestCharacterInString(input, position + 1, charAtPosition))
        {
            return EvaluativeSymbolTypes::DoubleCharacter;
        }

        return EvaluativeSymbolTypes::NotAnEvaluativeSymbol;
    }

    /// @brief Procedure in charge of maintaining scope-depth, parenthesis-depth and line-number based on detected character
    void ProcessLineAndScopeIncrement(char inputCharacter, uint32_t& lineNumberHolder, uint32_t& parenthesisDepthHolder, uint32_t& scopeDepthHolder)
    {
        switch (inputCharacter)
        {
            case '\n':
                lineNumberHolder++;
                break;

            case '{':
                scopeDepthHolder++;
                break;

            case '}':
                scopeDepthHolder--;
                break;

            case '(':
                parenthesisDepthHolder++;
                break;

            case ')':
                parenthesisDepthHolder--;
                break;

            default:
                break;
        }
    }

    /// @brief Alters the parser-engines state along with scan-position adjustement
    void SwitchToState(ParserStates& stateMachine, uint32_t& scanPosition, ParserLoopActionType scanPositionAdjustment)
    {
        switch (scanPositionAdjustment)
        {
            case ParserLoopActionType::StartFromSamePosition:       scanPosition -= 1; break;
            case ParserLoopActionType::StartFromOneCharacterAfter:  scanPosition += 0; break;
            case ParserLoopActionType::StartFromOneCharacterBefore: scanPosition -= 2; break;
            case ParserLoopActionType::StartFromTwoCharactersAfter: scanPosition += 1; break;
            default: throw CLexerException(PreliminaryParsingExceptionType::InternalParserError, "Unrecognized 'ParserLoopActionType' value.");
        }

        stateMachine = ParserStates::Default;
    };
}

std::shared_ptr<CRawFile> CLexer::ProcessLinkerScript(std::string absoluteFilePath, std::string rawContent)
{    
    // TODO: Revise code in event of available capacity to reduce cyclomatic complexity
    std::vector<CRawEntry> lexedContent;
    auto currentState = ParserStates::Default;    
    auto scopeDepth = (uint32_t)0; // Depth increases each time we encounter a '{' and decreases on each '}'
    auto parenthesisDepth = (uint32_t)0; // Depth increases each time we encounter a '(' and decreases on each ')'

    auto entryStartPosition = (uint32_t)0;   
    auto entryStartLine = (uint32_t)0;
    auto backslashArmed = false;     
    auto endOfStreamReached = false;
    auto lineNumber = (uint32_t)1;
    auto scanPosition = (uint32_t)0;
    auto supressLineAndScopeUpdate = false;

    auto nextCharacterExists = false;
    auto nextCharacter = ' ';

    auto previousCharacterExists = false;
    auto previousCharacter = ' ';

    for (scanPosition = 0;; scanPosition++)
    {        
        auto currentCharacter = rawContent[scanPosition];
        endOfStreamReached = scanPosition >= rawContent.length();

        nextCharacterExists = !endOfStreamReached;
        nextCharacter = (nextCharacterExists) ? rawContent[scanPosition + 1] : ' ';        

        switch (currentState)
        {
            case ParserStates::Default:
            {                
                if (IsWhitespace(currentCharacter))
                {
                    break;
                }

                if (IsNumber(currentCharacter))
                {     
                    entryStartPosition = scanPosition;
                    entryStartLine = lineNumber;
                    currentState = ParserStates::InNumber;
                    scanPosition--;
                    break;
                }

                if (SafeTestPatternInString(rawContent, scanPosition, '/', '*'))
                {
                    entryStartPosition = scanPosition;
                    entryStartLine = lineNumber;
                    currentState = ParserStates::InComment;
                    scanPosition--;
                    break;
                }

                if (IsDoubleQuotation(currentCharacter))
                {
                    entryStartPosition = scanPosition;
                    entryStartLine = lineNumber;
                    currentState = ParserStates::InString;
                    scanPosition--;
                    break;
                }

                auto assignmentSymbolType = TestForAssignmentSymbol(rawContent, scanPosition);
                if (assignmentSymbolType != AssignmentSymbolTypes::NotAnAssignmentSymbol)
                {
                    auto assignmentOperatorLength = static_cast<uint32_t>(assignmentSymbolType);
                    lexedContent.emplace_back(CRawEntry(RawEntryType::AssignmentOperator, lineNumber, scanPosition, assignmentOperatorLength, parenthesisDepth, scopeDepth));
                    scanPosition += (assignmentOperatorLength - 1);
                    break;
                }

                auto evaluatingSymbolType = TestForEvaluativeSymbol(rawContent, scanPosition);
                if (evaluatingSymbolType != EvaluativeSymbolTypes::NotAnEvaluativeSymbol)
                {
                    auto evaluatingSymbolTypeLength = static_cast<uint32_t>(evaluatingSymbolType);
                    lexedContent.emplace_back(CRawEntry(RawEntryType::EvaluativeOperators, lineNumber, scanPosition, evaluatingSymbolTypeLength, parenthesisDepth, scopeDepth));
                    scanPosition += (evaluatingSymbolTypeLength - 1);
                    break;
                }

                if (IsComma(currentCharacter))
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::Comma, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                    break;
                }

                if (IsSemicolon(currentCharacter))
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::Semicolon, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                    break;
                }

                if (IsColon(currentCharacter))
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::Colon, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                    break;
                }

                if (IsQuestionMark(currentCharacter))
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::QuestionMark, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                    break;
                }

                if (IsWildCardCharacter(currentCharacter))
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::Wildcard, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                    break;
                }

                uint32_t arithmeticOperatorLength;
                if (IsArithmeticOperator(currentCharacter, nextCharacter, arithmeticOperatorLength))
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::ArithmeticOperator, lineNumber, scanPosition, arithmeticOperatorLength, parenthesisDepth, scopeDepth));
                    scanPosition += arithmeticOperatorLength - 1;
                    break;
                }

                // Test against bracket and parenthesis
                {
                    auto entryType = (currentCharacter == '{') ? RawEntryType::BracketOpen :
                                     (currentCharacter == '}') ? RawEntryType::BracketClose :
                                     (currentCharacter == '(') ? RawEntryType::ParenthesisOpen :
                                     (currentCharacter == ')') ? RawEntryType::ParenthesisClose :
                                     RawEntryType::Unknown;

                    if (entryType != RawEntryType::Unknown)
                    {
                        lexedContent.emplace_back(CRawEntry(entryType, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                        break;
                    }
                }

                if (CanCharBeStartOfWord(currentCharacter))
                {
                    entryStartPosition = scanPosition;
                    entryStartLine = lineNumber;
                    currentState = ParserStates::InWord;
                    scanPosition--;                    
                }
                else // If all else failed, this character is unknown to us.
                {
                    lexedContent.emplace_back(CRawEntry(RawEntryType::Unknown, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                }

                break;
            }

            case ParserStates::InComment:
            {
                if (!SafeTestPatternInString(rawContent, scanPosition, '*', '/') && !endOfStreamReached)
                {
                    break;
                }

                lexedContent.emplace_back(CRawEntry(RawEntryType::Comment, entryStartLine, lineNumber, entryStartPosition, (scanPosition + 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromTwoCharactersAfter);
                break;
            }

            case ParserStates::InString:
            {   
                // Note: We accept the current character in the event that
                //       1 - It is not Double-Quotation (unless preceded by a back-slash)
                //       2 - End of file has not been reached
                //       3 - The character is not 'Line-Feed' as strings are not allowed to stretch across multiple lines
                if ((!IsDoubleQuotation(currentCharacter) || backslashArmed) && !endOfStreamReached && !IsLineFeed(currentCharacter))
                {
                    break;
                }

                // If 'Backslash', the next character should be taken at face-value.
                backslashArmed = (backslashArmed) ? false : (currentCharacter == '\\');

                // Note: Strings are always placed on the same line. A word cannot be split across two lines.
                lexedContent.emplace_back(CRawEntry(RawEntryType::String, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromOneCharacterAfter);
                break;
            }

            case ParserStates::InWord:
            {
                if (CanCharBeWithinWord(currentCharacter) && !endOfStreamReached) 
                {
                    break;
                }

                // Check for special case when a character is being quoted by '\'
                if (previousCharacterExists && previousCharacter == '\\')
                {
                    break;
                }

                // Note: Words are always placed on the same line. A word cannot be split across two lines.
                lexedContent.emplace_back(CRawEntry(RawEntryType::Word, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromSamePosition);
                supressLineAndScopeUpdate = true;
                break;
            }

            case ParserStates::InNumber:
            {
                if ((IsNumberPostfix(currentCharacter) || IsNumberPrefix(currentCharacter) || IsNumberOrHex(currentCharacter)) && !endOfStreamReached)
                {
                    break;
                }

                // Note: Numbers are always placed on the same line. A word cannot be split across two lines.
                lexedContent.emplace_back(CRawEntry(RawEntryType::Number, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromSamePosition);
                supressLineAndScopeUpdate = true;
                break;
            }

            default:
            {
                throw CLexerException(PreliminaryParsingExceptionType::InternalParserError, "ParserState was not recognized.");
            }
        }

        if (!supressLineAndScopeUpdate)
        {
            ProcessLineAndScopeIncrement(currentCharacter, lineNumber, parenthesisDepth, scopeDepth);
        }

        supressLineAndScopeUpdate = false;

        if (endOfStreamReached)
        {
            break;
        }

        previousCharacterExists = true;
        previousCharacter = currentCharacter;
    }

    std::string extractedFileName;
    std::vector<std::string> splittedString;

#ifdef COMPILING_FOR_WINDOWS
    splittedString = VisualLinkerScript::StringSplit(absoluteFilePath, '\\');
#elif COMPILING_FOR_UNIX_BASED
    auto splittedString = VisualLinkerScript::StringSplit(absoluteFilePath, '/');
#else
#error Either 'COMPILING_FOR_WINDOWS' need to be set or 'COMPILING_FOR_UNIX_BASED'
#endif

    extractedFileName = splittedString.back();

    return std::shared_ptr<CRawFile>(
                new CRawFile(std::move(rawContent),
                             extractedFileName,
                             absoluteFilePath,
                             std::move(lexedContent)));
}
