#include "CPreliminaryParser.h"
#include "CPreliminaryParsingException.h"
#include "Models/CRawEntry.h"
#include "Models/CRawFile.h"
#include "Models/EntryType.h"

#include <memory>
#include <string>
#include <vector>

using namespace VisualLinkerScript::ParsingEngine::Models;
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
    /// @param input Input string
    /// @param position Positions to check
    /// @param testAgainst Character to check against
    /// @return True if character matches, false if it doesn't or if position is out of range
    bool SafeTestCharacterInString(const string& input, unsigned int position, char testAgainst)
    {   
        return (position <= (input.length() - 1)) && (input[position] == testAgainst);
    }

    /// @brief Checks for the pattern of two characters at given positions. This is to optimize performance.
    /// @param input Input string
    /// @param position Position to start checking from
    /// @param firstCharacter First character to check (at 'position')
    /// @param secondCharacter Second character to check (at 'position+1')
    /// @return True if both characters found in sequence
    bool SafeTestPatternInString(
        const string& input, 
        unsigned int position, 
        const char firstCharacter,
        const char secondCharacter)
    {
        return SafeTestCharacterInString(input, position, firstCharacter) &&
               SafeTestCharacterInString(input, position + 1, secondCharacter);
    }

    /// @brief Checks for the pattern of two characters at given positions. This is to optimize performance.
    /// @param input Input string
    /// @param position Position to start checking from
    /// @param firstCharacter First character to check (at 'position')
    /// @param secondCharacter Second character to check (at 'position+1')
    /// @param thirdCharacter Third character to check (at 'position+1')
    /// @return True if both characters found in sequence
    bool SafeTestPatternInString(
        const string& input, 
        unsigned int position, 
        const char firstCharacter, 
        const char secondCharacter, 
        const char thirdCharacter)
    {
        return SafeTestCharacterInString(input, position, firstCharacter) &&
               SafeTestCharacterInString(input, position + 1, secondCharacter) &&
               SafeTestCharacterInString(input, position + 2, thirdCharacter);
    }

    /// @brief Determines if this character can be the first letter of a word
    /// @param input Character to check
    /// @return True if the character can be the first letter of a word (i.e. A-Z, a-z, '_')
    bool CanCharBeStartOfWord(const char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||
               (input == '_') ||
               (input == '?') ||                    // Wildcard support
               (input == '*') ||                    // Wildcard support
               (input == '[') ||                    // Wildcard support
               (input == ']') ||                    // Wildcard support
               (input == '/') ||                    // Wildcard support               
               (input == '.');                      // Can also be part of a word
    }

    /// @brief Checks to see if a given character can be an operator
    /// @param input Input to check
    /// @return True if character is an operator (although further checks will be needed for some)
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
    /// @param input Character to check
    /// @return True if the character can be considered as part of word (i.e. A-Z, a-z, 0-9 and '_')
    bool CanCharBeWithinWord(const char input)
    {
        return ((input >= '0') && (input <= '9')) ||
               CanCharBeStartOfWord(input);
    }


    /// @brief Checks the character for being a potential prefix (such as 0x, 0b, etc.)
    /// @param input Character to check
    /// @return True if a prefix-character is observed.
    bool IsNumberPrefix(char input)
    {
        return ((input == 'x') ||
                (input == 'X') ||
                (input == 'b') ||
                (input == 'B'));
    }

    /// @brief Checks the character for being a potential postfixes ('K' and 'M')
    /// @param input Character to check
    /// @return True if a post-fix character is observed.
    bool IsNumberPostfix(char input)
    {
        return ((input == 'K') ||
                (input == 'M'));
    }

    /// @brief Detects if the input character is a white-space
    /// @param input Input to check.
    /// @return True if 'input' is a white-space character. False otherwise.
    bool IsWhitespace(char input)
    {
        return ((input >= (char)0x09) && (input <= (char)0x0D)) || (input == ' ');
    }

    /// @brief Checks if input is a digit. 
    /// @param input Input to check.
    /// @return Self explanatory.
    bool IsNumber(char input)
    {
        return (input >= '0') && (input <= '9');
    }


    /// @brief Checks if input is a digit or a hexadecimal. 
    /// @param input Input to check.
    /// @return Self explanatory.
    bool IsNumberOrHex(char input)
    {
        return ((input >= '0') && (input <= '9')) || 
               ((input >= 'A') && (input <= 'F')) ||
               ((input >= 'a') && (input <= 'f'));
    }

    /// @brief Checks to see if assignment symbol is present, 
    ///        such as '/=', '*=', '+=', '-=', '='
    /// @param input Input string to check
    /// @param position Position to check at
    /// @return Returns the type of detected assignment symbol
    AssignmentSymbolTypes TestForAssignmentSymbol(const string& input, const unsigned int position)
    {
        auto charAtPosition = input[position];
        auto detectedOperatorLength = (unsigned int)0;

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
                AssignmentSymbolTypes::NotAnAssignmentSymbol;
            }

            detectedOperatorLength = IsWhitespace(input[position+2]) ? 2 : 0;
        }

        return (detectedOperatorLength == 1) ? AssignmentSymbolTypes::SingleCharacter :
               (detectedOperatorLength == 2) ? AssignmentSymbolTypes::DoubleCharacter :
               AssignmentSymbolTypes::NotAnAssignmentSymbol ;
    }

    /// @brief Procedure in charge of maintaining scope-depth, parenthesis-depth and line-number based on detected character
    void ProcessLineAndScopeIncrement(char inputCharacter, unsigned int& lineNumberHolder, unsigned int& parenthesisDepthHolder, unsigned int& scopeDepthHolder)
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
            default: throw CPreliminaryParsingException(PreliminaryParsingExceptionTypeEnum::InternalParserError, "Unrecognized 'ParserLoopActionType' value.");
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

std::shared_ptr<CRawFile> CPreliminaryParser::ProcessLinkerScript(const std::string& rawContent)
{
    auto parsedContent = std::make_shared<std::vector<CRawEntry>>();
    auto currentState = ParserStates::Default;    
    auto scopeDepth = (unsigned int)0; // Depth increases each time we encounter a '{' and decreases on each '}'
    auto parenthesisDepth = (unsigned int)0; // Depth increases each time we encounter a '(' and decreases on each ')'

    auto entryStartPosition = (unsigned int)0;   
    auto entryStartLine = (unsigned int)0;
    auto backslashArmed = false;     
    auto endOfStreamReached = false;
    auto lineNumber = (unsigned int)1;
    auto scanPosition = (int)0;
    auto supressLineAndScopeUpdate = false;

    for (scanPosition = 0;; scanPosition++)
    {        
        auto currentCharacter = rawContent[scanPosition];
        endOfStreamReached = scanPosition >= rawContent.length();

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

                if (currentCharacter == '"')
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
                    parsedContent->emplace_back(CRawEntry(EntryType::Assignment, lineNumber, lineNumber, scanPosition, assignmentOperatorLength, parenthesisDepth, scopeDepth));
                    scanPosition += (assignmentOperatorLength - 1);
                    break;
                }

                if (CanCharBeStartOfWord(currentCharacter))
                {
                    entryStartPosition = scanPosition;
                    entryStartLine = lineNumber;
                    currentState = ParserStates::InWord;
                    scanPosition--;
                    break;
                }

                if (IsOperator(currentCharacter))
                {
                    auto entyType = (currentCharacter == '{') ? EntryType::BracketOpen :
                                    (currentCharacter == '}') ? EntryType::BracketClose :
                                    (currentCharacter == '(') ? EntryType::ParenthesisOpen :
                                    (currentCharacter == ')') ? EntryType::ParenthesisClose :
                                    EntryType::Operator;

                    parsedContent->emplace_back(CRawEntry(entyType, lineNumber, lineNumber, scanPosition, 1, parenthesisDepth, scopeDepth));
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

                parsedContent->emplace_back(CRawEntry(EntryType::Comment, entryStartLine, lineNumber, entryStartPosition, (scanPosition + 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromTwoCharactersAfter);
                break;
            }

            case ParserStates::InString:
            {                
                backslashArmed = (backslashArmed) ? false : (currentCharacter == '\\');                
                if (((currentCharacter != '"') || backslashArmed) && !endOfStreamReached)
                {
                    break;
                }

                // Note: Strings are always placed on the same line. A word cannot be split across two lines.
                parsedContent->emplace_back(CRawEntry(EntryType::String, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
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
                parsedContent->emplace_back(CRawEntry(EntryType::Word, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
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
                parsedContent->emplace_back(CRawEntry(EntryType::Number, entryStartLine, entryStartLine, entryStartPosition, (scanPosition - 1 - entryStartPosition) + 1, parenthesisDepth, scopeDepth));
                SwitchToState(currentState, scanPosition, ParserLoopActionType::StartFromSamePosition);
                supressLineAndScopeUpdate = true;
                break;
            }

            default:
            {
                throw CPreliminaryParsingException(PreliminaryParsingExceptionTypeEnum::InternalParserError, "ParserState was not recognized.");
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

    return std::make_shared<CRawFile>(parsedContent);
}
