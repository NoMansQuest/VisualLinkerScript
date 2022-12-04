#include "CPreliminaryParser.h"
#include "CPreliminaryParsingException.h"
#include "Models/CRawEntry.h"
#include "Models/CRawFile.h"
#include "Models/EntryTypes.h"

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
        InWhiteSpace,
        InString,
        InWord,
        InNumber,
    };

    /// @brief Checks if the character at the given position matches @see testAgainst.
    /// @param input Input string
    /// @param position Positions to check
    /// @param testAgainst Character to check against
    /// @return True if character matches, false if it doesn't or if position is out of range
    bool SafeTestCharacterInString(const string& input, unsigned int position, char testAgainst)
    {   
        return (position <= (input.length - 1)) && 
               (input[position] == testAgainst);        
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
                (input == '~') ||
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
        return (input >= (char)0x09) && (input <= (char)0x0D);
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

    /// @brief  Checks the character for being an operator ( & = &= += > >> .... )
    /// @param input Character to check
    /// @return True if the character is considered to be an operator
    bool IsOperator(char input)
    {
        return ((input == ':') ||
                (input == '!') ||
                (input == '*') ||
                (input == '/') ||
                (input == '%') ||
                (input == '+') ||
                (input == '-') ||
                (input == '~') ||
                (input == '&') ||
                (input == '=') ||
                (input == '*') ||
                (input == '?'));
    }

    /// @brief Checks to see if assignment symbol is present, 
    ///        such as '/=', '*=', '+=', '-=', '='
    /// @param input Input string to check
    /// @param position Position to check at
    /// @return 
    bool TestForAssignmentSymbol(const string& input, unsigned int position)
    {
        if (SafeTestCharacterInString(input, position) == '')
    }
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
    auto sectionStartIndex = (unsigned int)0;
    auto currentDepth = (unsigned int)0; // Depth increases each time we encounter a '{' and decreases on each '}'

    auto eventStartPosition = (unsigned int)0;    
    
    for (unsigned int hoverPosition = 0;; hoverPosition++)
    {
        auto currentCharacter = rawContent[hoverPosition];
        switch (currentState)
        {
            case ParserStates::Default:
            {
                if (IsWhitespace(currentCharacter))
                {
                    continue;
                }

                if (IsNumber(hoverPosition))
                {                   
                    eventStartPosition = hoverPosition; 
                    hoverPosition--;                    
                    currentState = ParserStates::InNumber;
                    continue;
                }

                if (SafeTestPatternInString(rawContent, hoverPosition, '/', '*'))
                {
                    eventStartPosition = hoverPosition;
                    hoverPosition--;
                    currentState = ParserStates::InComment;
                    continue;
                }

                if (IsAssignment(rawContent, hoverPosition))
                {

                }
            }

            case ParserStates::InComment:
            {
                if (!SafeTestPatternInString(rawContent, hoverPosition, '*', '/'))
                {
                    continue;
                }

                parsedContent->emplace_back(CRawEntry(EntryTypesEnum::Comment,
                                                      eventStartPosition,
                                                      (hoverPosition + 1 - eventStartPosition) + 1,
                                                      currentDepth));
                hoverPosition++;
                currentState = ParserStates::Default;
            }

            case ParserStates::InWhiteSpace:
            {

            }

            case ParserStates::InString:
            {

            }

            case ParserStates::InWord:
            {

            }

            case ParserStates::InNumber:
            {
                if (!IsNumberPostfix(currentCharacter) &&
                    !IsNumberPrefix(currentCharacter) &&
                    !IsNumber

            }

            default:
            {


            }
        }
    }

    return std::make_shared<CRawFile>(parsedContent);
}
