#include "CPreliminaryParser.h"
#include "CPreliminaryParsingException.h"
#include "Models/Raw/CRawEntry.h"
#include "Models/Raw/CRawFile.h"
#include "Models/Raw/RawEntryType.h"

#include <memory>
#include <string>
#include <vector>

using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
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

    /// @brief Types of assignment symbols we can have in a linker-script
    enum class AssignmentSymbolTypes
    {
        SingleCharacter,
        DoubleCharacter,
        NotAnAssignmentSymbol
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
    bool SafeTestCharacterInString(const string& input, uint32_t position, char testAgainst)
    {   
        return (position <= (input.length() - 1)) && (input[position] == testAgainst);
    }

    /// @brief Checks for the pattern of two characters at given positions. This is to optimize performance.
    bool SafeTestPatternInString(
        const string& input, 
        uint32_t position, 
        const char firstCharacter,
        const char secondCharacter)
    {
        return SafeTestCharacterInString(input, position, firstCharacter) &&
               SafeTestCharacterInString(input, position + 1, secondCharacter);
    }

    /// @brief Checks for the pattern of two characters at given positions. This is to optimize performance.
    bool SafeTestPatternInString(
        const string& input, 
        uint32_t position, 
        const char firstCharacter, 
        const char secondCharacter, 
        const char thirdCharacter)
    {
        return SafeTestCharacterInString(input, position, firstCharacter) &&
               SafeTestCharacterInString(input, position + 1, secondCharacter) &&
               SafeTestCharacterInString(input, position + 2, thirdCharacter);
    }

    /// @brief Determines if this character can be the first letter of a word
    bool CanCharBeStartOfWord(const char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||
               (input == '_') ||
               (input == '>') ||   // For example ">AT(SRAM)" placed after an output-section
               (input == ':') ||   // For example ":phdrs" placed after an output-section
               (input == '?') ||   // Wildcard support
               (input == '*') ||   // Wildcard support, could also be an operator
               (input == '[') ||   // Wildcard support
               (input == ']') ||   // Wildcard support
               (input == '/') ||   // Wildcard support, could also be an operator (e.g. /DISCARD/)
               (input == '!') ||   // Wildcard support, could also be an operator (e.g. Memory-Statement attribute '(!r)')
               (input == '.');     // Wildcard support, could also be an operator
    }

    /// @brief Checks to see if a given character can be an operator
    bool IsOperator(const char input)
    {   
        return ((input == '*') ||
                (input == '>') ||
                (input == '<') ||
                (input == '/') ||
                (input == '-') ||
                (input == '+') ||
                (input == '&') ||
                (input == '%') ||
                (input == '=') ||
                (input == '?') ||
                (input == ':') ||
                (input == ',') ||
                (input == ';') ||
                (input == '~') ||
                (input == '(') ||
                (input == ')') ||
                (input == '{') ||
                (input == '}') ||
                (input == '!'));
    }

    /// @brief If true, then the character can be considered as part of a word
    bool CanCharBeWithinWord(char input)
    {
        return ((input >= '0') && (input <= '9')) ||
               CanCharBeStartOfWord(input);
    }


    /// @brief Checks the character for being a potential prefix (such as 0x, 0b, etc.)
    bool IsNumberPrefix(char input)
    {
        return ((input == 'x') ||
                (input == 'X') ||
                (input == 'b') ||
                (input == 'B'));
    }

    /// @brief Checks the character for being a potential postfixes ('K' and 'M')
    bool IsNumberPostfix(char input)
    {
        return ((input == 'K') ||
                (input == 'M'));
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

    /// @brief Checks if a long operator (e.g. >>= or !=, etc.) is present at position
    bool CheckForLongOperator(const std::string& rawContent, const uint32_t position, uint32_t& longOperatorLength)
    {
        std::vector<std::string> recognizedLongOperators
        {
          ">>= ", "<<= ", "!= " , "== " , "+= " , "-= " , "*= " , "/= " , ">> " , "<< "
        };

        for (auto operatorToCheckFor: recognizedLongOperators)
        {
            if (rawContent.rfind(operatorToCheckFor, position) != std::string::npos)
            {
               longOperatorLength = operatorToCheckFor.length();
               return true;
            }
        }

        return false;
    }

    /// @brief Checks to see if assignment symbol is present, 
    ///        such as '/=', '*=', '+=', '-=', '='
    AssignmentSymbolTypes TestForAssignmentSymbol(const string& input, const uint32_t position)
    {
        auto charAtPosition = input[position];
        auto detectedOperatorLength = (uint32_t)0;

        if (SafeTestCharacterInString(input, position, '='))
        {
            if (position == input.length() - 1)
            {
                return AssignmentSymbolTypes::SingleCharacter;
            }

            detectedOperatorLength = IsWhitespace(input[position+1]) ? 1 : 0;            
        }
        else if ((charAtPosition == '*') ||
                 (charAtPosition == '/') ||
                 (charAtPosition == '%') ||
                 (charAtPosition == '-') ||
                 (charAtPosition == '+'))
        {
            if (position == input.length() - 1)
            {
                return AssignmentSymbolTypes::NotAnAssignmentSymbol;
            }

            if (!SafeTestCharacterInString(input, position + 1, '=') || (position == input.length() - 2))
            {
                return AssignmentSymbolTypes::NotAnAssignmentSymbol;
            }

            detectedOperatorLength = IsWhitespace(input[position+2]) ? 2 : 0;
        }

        return (detectedOperatorLength == 1) ? AssignmentSymbolTypes::SingleCharacter :
               (detectedOperatorLength == 2) ? AssignmentSymbolTypes::DoubleCharacter :
               AssignmentSymbolTypes::NotAnAssignmentSymbol ;
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
    void SwitchToState(ParserStates& stateMachine, int& scanPosition, const ParserLoopActionType scanPositionAdjustment)
    {
        switch (scanPositionAdjustment)
        {
            case ParserLoopActionType::StartFromSamePosition:       scanPosition -= 1; break;
            case ParserLoopActionType::StartFromOneCharacterAfter:  scanPosition += 0; break;
            case ParserLoopActionType::StartFromOneCharacterBefore: scanPosition -= 2; break;
            case ParserLoopActionType::StartFromTwoCharactersAfter: scanPosition += 1; break;
            default: throw CPreliminaryParsingException(PreliminaryParsingExceptionType::InternalParserError, "Unrecognized 'ParserLoopActionType' value.");
        }

        stateMachine = ParserStates::Default;
    };
}

CPreliminaryParser::CPreliminaryParser()
{    
    // No action needed at this point in time.
}

CPreliminaryParser::~CPreliminaryParser()
{
    // No action needed at this point in time.
}

std::shared_ptr<CRawFile> CPreliminaryParser::ProcessLinkerScript(const std::string& fileName, const std::string& rawContent)
{    
    // TODO: Revise code in event of available capacity to reduce cyclomatic complexity

    std::vector<CRawEntry> parsedContent;
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

    for (scanPosition = 0;; scanPosition++)
    {        
        auto currentCharacter = rawContent[scanPosition];
        endOfStreamReached = scanPosition >= rawContent.length();

        nextCharacterExists = !endOfStreamReached;
        nextCharacter = (nextCharacterExists) ? rawContent[scanPosition + 1] : ' ';

        std::string insightScope = rawContent.substr(scanPosition);

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
                    auto assignmentOperatorLength = (assignmentSymbolType == AssignmentSymbolTypes::SingleCharacter) ? 1 : 2;
                    parsedContent.emplace_back(CRawEntry(RawEntryType::Assignment, lineNumber, scanPosition, assignmentOperatorLength, parenthesisDepth, scopeDepth));
                    scanPosition += (assignmentOperatorLength - 1);
                    break;
                }

                if (CanCharBeStartOfWord(currentCharacter))
                {
                    uint32_t longOperatorLength;
                    if (IsOperator(currentCharacter) && CheckForLongOperator(rawContent, scanPosition, longOperatorLength))
                    {
                        parsedContent.emplace_back(CRawEntry(RawEntryType::Operator, lineNumber, scanPosition, longOperatorLength, parenthesisDepth, scopeDepth));
                        scanPosition += longOperatorLength - 1;
                        break;
                    }
                    else
                    {
                        entryStartPosition = scanPosition;
                        entryStartLine = lineNumber;
                        currentState = ParserStates::InWord;
                        scanPosition--;
                        break;
                    }
                }

                if (IsOperator(currentCharacter))
                {
                    auto entyType = (currentCharacter == '{') ? RawEntryType::BracketOpen :
                                    (currentCharacter == '}') ? RawEntryType::BracketClose :
                                    (currentCharacter == '(') ? RawEntryType::ParenthesisOpen :
                                    (currentCharacter == ')') ? RawEntryType::ParenthesisClose :
                                    RawEntryType::Operator;

                    parsedContent.emplace_back(CRawEntry(entyType, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
                    break;
                }

                break;
            }

            case ParserStates::InComment:
            {
                if (!SafeTestPatternInString(rawContent, scanPosition, '*', '/') && !endOfStreamReached)
                {
                    break;
                }

                parsedContent.emplace_back(CRawEntry(RawEntryType::Comment, entryStartLine, lineNumber, entryStartPosition, (scanPosition + 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
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
                parsedContent.emplace_back(CRawEntry(RawEntryType::String, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromOneCharacterAfter);
                break;
            }

            case ParserStates::InWord:
            {
                if (CanCharBeWithinWord(currentCharacter) && !endOfStreamReached) 
                {
                    break;
                }

                // Note: Words are always placed on the same line. A word cannot be split across two lines.
                parsedContent.emplace_back(CRawEntry(RawEntryType::Word, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
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
                parsedContent.emplace_back(CRawEntry(RawEntryType::Number, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromSamePosition);
                supressLineAndScopeUpdate = true;
                break;
            }

            default:
            {
                throw CPreliminaryParsingException(PreliminaryParsingExceptionType::InternalParserError, "ParserState was not recognized.");
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
    }

    return std::make_shared<CRawFile>(std::move(rawContent), fileName, std::move(parsedContent));
}
