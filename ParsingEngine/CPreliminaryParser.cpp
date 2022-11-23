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
        InWhiteSpace,
        InString,
        InWord,
        InNumber,
    };

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
               (input == '-') || (input == '+') ||  // Yes, weirdly enough these are allowed in LinkerScript symbol names!
               (input == '.');                      // Can also be part of a word
    }

    /// @brief If true, then the character can be considered as part of a word
    /// @param input Character to check
    /// @return True if the character can be considered as part of word (i.e. A-Z, a-z, 0-9 and '_')
    bool CanCharBeWithinWord(const char input)
    {
        return ((input >= '0') && (input <= '9')) ||
               CanCharBeStartOfWord(input);
    }


    /// @brief  Checks the character for being a potential prefix (such as 0x, 0b, etc.)
    /// @param input Character to check
    /// @return True if a prefix-character is observed.
    bool IsNumberPrefix(char input)
    {
        return ((input == 'x') ||
                (input == 'X') ||
                (input == 'b') ||
                (input == 'B'));
    }

    /// @brief  Checks the character for being a potential postfixes ('K' and 'M')
    /// @param input Character to check
    /// @return True if a post-fix character is observed.
    bool IsNumberPostfix(char input)
    {
        return ((input == 'K') ||
                (input == 'M'));
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
                (input == '*') ||
                (input == '?'));
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

    for (unsigned int hoverPosition = 0 ;; hoverPosition++)
    {

    }

    return std::make_shared<CRawFile>(parsedContent);
}
