#include <cstdarg>
#include <memory>
#include <string>
#include <vector>

#include "CLexer.h"
#include <QDebug>
#include <QString>

#include <ostream>

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
    enum class LexerStates
    {
        Default,
        InComment,        
        InString,
        InWord,
        InNumber,
    };

    /// @brief Types of assignment symbols we can have in a linker-script (i.e. =, +=, -=, &=, >>=, etc.)
    enum class AssignmentSymbolTypes : int32_t
    {
        SingleCharacter = 1,
        DoubleCharacter = 2,
        TripleCharacter = 3,
        NotAnAssignmentSymbol = 4
    };

    /// @brief Types of evaluative symbols we can have in a linker-script (i.e. >=, <=, ==, etc.)
    enum class EvaluativeSymbolTypes : int32_t
    {
        SingleCharacter = 1,
        DoubleCharacter = 2,
        NotAnEvaluativeSymbol = 3
    };

    /// @brief The loop needs alignment after an entry is detected and registered
    enum class ParserLoopActionType
    {
        StartFromSamePosition,
        StartFromOneCharacterAfter,
        StartFromOneCharacterBefore,
        StartFromTwoCharactersAfter
    };

    /// @brief Structure holds data relevant to the start of a blocked section (i.e. String, Commands, Words, etc.).
    struct LexerPosition
    {
        uint32_t AbsolutePosition;
        uint32_t LineNumber;
        uint32_t IndexInLine;
        LexerStates State;
        uint32_t ScopeDepth;
        uint32_t ParenthesisDepth;
        uint32_t LineStartScopeDepth;

        void AdvanceByLineFeed()
        {
            ++this->AbsolutePosition;
            ++this->LineNumber;
            this->IndexInLine = 0;
            this->LineStartScopeDepth = this->ScopeDepth;
        }

        void AdvanceByLineFeed(const LexerStates newLexerState)
        {
            this->AdvanceByLineFeed();
            this->State = newLexerState;
        }

        void Advance(const uint32_t movement)
        {
            this->AbsolutePosition += movement;
            this->IndexInLine += movement;
        }

        void Advance(const uint32_t movement, const LexerStates newLexerState)
        {
            this->AbsolutePosition += movement;
            this->IndexInLine += movement;
            this->State = newLexerState;
        }

        LexerPosition Clone(const int32_t movement) const
        {
            auto absolutePositionPostAdjustmentResult = static_cast<int32_t>(this->AbsolutePosition) + movement;
            auto indexInLinePostAdjustmentResult = static_cast<int32_t>(this->IndexInLine) + movement;

            if (absolutePositionPostAdjustmentResult < 0)
            {
                throw std::exception("AbsolutePosition, after adjustment, became negative!");
            }

            if (indexInLinePostAdjustmentResult < 0)
            {
                throw std::exception("IndexInLine, after adjustment, became negative!");
            }

            return LexerPosition{
            	static_cast<uint32_t>(absolutePositionPostAdjustmentResult),
            	this->LineNumber,
            	static_cast<uint32_t>(indexInLinePostAdjustmentResult),
            	this->State,
            	this->ScopeDepth,
            	this->ParenthesisDepth,
            	this->LineStartScopeDepth };
        }

        void SafeAddScopeDepth(int32_t scopeDepthToAdd)
        {
            auto postAdjustmentResult = static_cast<int32_t>(this->ScopeDepth) + scopeDepthToAdd;
            this->ScopeDepth = static_cast<uint32_t>(std::max(postAdjustmentResult,0));
        }

        void SafeAddParenthesisDepth(int32_t parenthesisDepthToAdd)
        {
            auto postAdjustmentResult = static_cast<int32_t>(this->ParenthesisDepth) + parenthesisDepthToAdd;
            this->ParenthesisDepth = static_cast<uint32_t>(std::max(postAdjustmentResult, 0));
        }

        void UpdatedState(const LexerStates newLexerState)
        {
            this->State = newLexerState;
        }

        static uint32_t LengthDiff(const LexerPosition& endPosition, const LexerPosition& startPosition)
        {
            return endPosition.AbsolutePosition - startPosition.AbsolutePosition + 1;
        }
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
        const std::vector<char>& characterSequence)
    {
        for (const auto element : characterSequence)
        {
           if ((position >= input.length()) || (input[position] != element))
           {
               return false;
           }
           position++;
        }
        return true;
    }

    /// @brief Determines if this character can be the first letter of a word
    bool CanCharBeStartOfWord(const char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||
               (input == '_')  || (input == '.');     // Wildcard support, could also be an operator
    }

    /// @brief If true, then the character can be considered as part of a word
    bool CanCharBeWithinWord(const char input)
    {
        return ((input >= '0') && (input <= '9')) || (input == '\\') || CanCharBeStartOfWord(input);
    }

    /// @brief Checks to see if a given characters make an 'L-Shift' ir 'R-Shift' operator
    bool IsArithmeticShiftOperator(const char input, const char inputPlusOne)
    {
        return ((input == '>' && inputPlusOne == '>') ||
				(input == '<' && inputPlusOne == '<'));
    }

    /// @brief Checks to see if a given character can be an arithmetic operator.
    ///        These include right-shift and left-shift operators ('<<' and '>>').
    bool IsArithmeticOperator(const char input)
    {
        return ((input == '*') || (input == '/') || (input == '&') || (input == '|') ||
                (input == '-') || (input == '+') || (input == '~') || (input == '!') ||
                (input == '%') || (input == '^'));
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
    bool IsNumberPrefix(const char input)
    {
        return ((input == 'x') || (input == 'X') || (input == 'b') || (input == 'B'));
    }

    /// @brief Checks the character for being a potential postfixes ('K' and 'M')
    bool IsNumberPostfix(const char input)
    {
        return ((input == 'K') ||
                (input == 'M'));
    }

    /// @brief The following characters are considered wild-card characters. The list here is
    ///        not exhaustive (*, ? and - can also be wildcards), however the other character's 
    ///        role as wild-card as subjective. 
    /// @param input 
    /// @return 
    bool IsWildCardCharacter(const char input)
    {
        return (input == '[') ||
               (input == ']') ||
               (input == '\\');               
    }

    /// @brief Detects if the input character is a white-space
    bool IsWhitespace(const char input)
    {
        return ((input >= 0x09) && (input <= 0x0D)) || (input == ' ');
    }

    /// @brief Checks if input is a digit. 
    bool IsNumber(char input)
    {
        return (input >= '0') && (input <= '9');
    }

    /// @brief Checks if input is a digit or a hexadecimal. 
    bool IsNumberOrHex(const char input)
    {
        return ((input >= '0') && (input <= '9')) || 
               ((input >= 'A') && (input <= 'F')) ||
               ((input >= 'a') && (input <= 'f'));
    }

    /// @brief Checks if input is a double-quotation symbo, marking the start/end of a string
    bool IsDoubleQuotation(const char input)
    {
        return (input == '"');
    }

    /// @brief Checks if input is Line-Feed symbol
    bool IsLineFeed(const char input)
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

        if ((charAtPosition == '*') || (charAtPosition == '/') || (charAtPosition == '%') || (charAtPosition == '-') ||
	        (charAtPosition == '~') || (charAtPosition == '|') || (charAtPosition == '&') || (charAtPosition == '^') ||
	        (charAtPosition == '+'))
        {
	        return (!SafeTestCharacterInString(input, position + 1, '=')) ?
		               AssignmentSymbolTypes::NotAnAssignmentSymbol :
		               AssignmentSymbolTypes::DoubleCharacter;
        }

        if ((charAtPosition == '>') || (charAtPosition == '<'))
        {
	        bool isAssignment = (SafeTestCharacterInString(input, position + 1, charAtPosition) &&
		        SafeTestCharacterInString(input, position + 2, '='));

	        return isAssignment ? AssignmentSymbolTypes::TripleCharacter : AssignmentSymbolTypes::NotAnAssignmentSymbol;
        }

        return AssignmentSymbolTypes::NotAnAssignmentSymbol;
    }

    /// @brief Tests character against single-character punctuates and wildcards
    RawEntryType TestForStatementPunctuatesAndWildcards(const char characterToTest)
    {
        if (IsComma(characterToTest))
        {
            return RawEntryType::Comma;
        }

        if (IsSemicolon(characterToTest))
        {
            return RawEntryType::Semicolon;
        }

        if (IsColon(characterToTest))
        {
            return RawEntryType::Colon;
        }

        if (IsQuestionMark(characterToTest))
        {
            return RawEntryType::QuestionMark;
        }

        if (IsWildCardCharacter(characterToTest))
        {
            return RawEntryType::Wildcard;
        }

        return RawEntryType::Unknown;
    }

    /// @brief Tests character against scope and depth punctuates
    RawEntryType TestForScopeDepthPunctuates(const char characterToTest)
    {
        return (characterToTest == '{') ? RawEntryType::BracketOpen :
			   (characterToTest == '}') ? RawEntryType::BracketClose :
			   (characterToTest == '(') ? RawEntryType::ParenthesisOpen :
			   (characterToTest == ')') ? RawEntryType::ParenthesisClose :
			   RawEntryType::Unknown;
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
                   EvaluativeSymbolTypes::DoubleCharacter :
                   EvaluativeSymbolTypes::NotAnEvaluativeSymbol ;
        }
        else if ((charAtPosition == '|' || charAtPosition == '&') && SafeTestCharacterInString(input, position + 1, charAtPosition))
        {
            return EvaluativeSymbolTypes::DoubleCharacter;
        }

        return EvaluativeSymbolTypes::NotAnEvaluativeSymbol;
    }

    /// @brief Generic function to insert multi-character lexer content. This is a massive help to code readability.
    void AddLexerContent(
        std::vector<CRawEntry>& contentVector, 
        const RawEntryType entryType, 
        const LexerPosition& startPosition, 
        const LexerPosition& endPosition)
    {
        contentVector.emplace_back(
            entryType,
            startPosition.LineNumber,
            startPosition.IndexInLine,
            endPosition.LineNumber,
            endPosition.IndexInLine,
            startPosition.AbsolutePosition,
            endPosition.AbsolutePosition - startPosition.AbsolutePosition + 1,
            startPosition.ParenthesisDepth,
            startPosition.ScopeDepth);
    }

    /// @brief Generic function to insert single-character lexer content. This is a massive help to code readability.
    void AddLexerContent(
        std::vector<CRawEntry>& contentVector,
        const RawEntryType entryType,
        const LexerPosition& startPosition)
    {
        contentVector.emplace_back(
            entryType,
            startPosition.LineNumber,
            startPosition.IndexInLine,
            startPosition.LineNumber,
            startPosition.IndexInLine,
            startPosition.AbsolutePosition,
            1,
            startPosition.ParenthesisDepth,
            startPosition.ScopeDepth);
    }

}

std::shared_ptr<CRawFile> CLexer::LexLinkerScript(const std::string& absoluteFilePath, std::string rawContent)
{    
    // TODO: Revise code in event of available capacity to reduce cyclomatic complexity
    std::vector<CRawEntry> lexerContent;
    std::unordered_map<uint32_t, CIndentationInfo> lineIndentationMap;
    LexerPosition lexerContext { 0, 0, 0, LexerStates::Default, 0, 0, 0};
    LexerPosition blockStartPosition;
    auto endOfStreamReached = false;

    do
    {
        endOfStreamReached = lexerContext.AbsolutePosition + 1 > rawContent.length();
        auto nextCharacter = (lexerContext.AbsolutePosition + 1 < rawContent.length()) ? rawContent[lexerContext.AbsolutePosition + 1] : ' ';
        auto previousCharacter = lexerContext.AbsolutePosition > 0 ? rawContent[lexerContext.AbsolutePosition - 1] : ' ';
        auto currentCharacter = !endOfStreamReached ? rawContent[lexerContext.AbsolutePosition] : ' ';
        auto isLineFeed = IsLineFeed(currentCharacter);

        // We assign the ScopeDepth to a line under two scenarios: End-Of-Stream and New-Line
        if (endOfStreamReached || isLineFeed)
        {
            auto scopeDepthToAssign = std::min(lexerContext.ScopeDepth, lexerContext.LineStartScopeDepth);
            auto commentLinePrefixingNeeded = (lexerContext.State == LexerStates::InComment) && (blockStartPosition.LineNumber != lexerContext.LineNumber);
            auto indentationType = (lexerContext.State == LexerStates::InComment) ? EIndentationLineType::MultiLineCommentStar : EIndentationLineType::RegularSpace;
            lineIndentationMap.emplace(lexerContext.LineNumber, CIndentationInfo(scopeDepthToAssign, indentationType, commentLinePrefixingNeeded));
        }

        // Process the current character
        switch (lexerContext.State)
        {
            case LexerStates::Default:
            {
                if (IsWhitespace(currentCharacter))
                {
                    if (isLineFeed)
                    {
                        lexerContext.AdvanceByLineFeed();
                    }
                    else
                    {
                        lexerContext.Advance(1);
                    }
                    continue;
                }

                if (IsNumber(currentCharacter))
                {
                    blockStartPosition = lexerContext; // Save this position
                    lexerContext.Advance(1, LexerStates::InNumber);                    
                    continue;
                }

                if (SafeTestPatternInString(rawContent, lexerContext.AbsolutePosition, { '/', '*' }))
                {
                    blockStartPosition = lexerContext; // Save this position                    
                    lexerContext.Advance(1, LexerStates::InComment);
                    continue;
                }

                if (SafeTestPatternInString(rawContent, lexerContext.AbsolutePosition, { '/', 'D', 'I', 'S', 'C', 'A', 'R', 'D', '/' }))
                {
                    auto wordLength = 9;
                    auto endWordPosition = lexerContext.Clone(wordLength - 1);
                    AddLexerContent(lexerContent, RawEntryType::Word, lexerContext, endWordPosition);
                    lexerContext.Advance(wordLength); // Move lexer to the character after the word (= length).
                    continue;
                }

                if (IsDoubleQuotation(currentCharacter))
                {
                    blockStartPosition = lexerContext;
                    lexerContext.Advance(1, LexerStates::InString);
                    continue;
                }

                auto assignmentSymbolType = TestForAssignmentSymbol(rawContent, lexerContext.AbsolutePosition);
                if (assignmentSymbolType != AssignmentSymbolTypes::NotAnAssignmentSymbol)
                {
                    auto assignmentOperatorLength = static_cast<int32_t>(assignmentSymbolType);
                    auto endWordPosition = lexerContext.Clone(assignmentOperatorLength - 1);
                    AddLexerContent(lexerContent, RawEntryType::AssignmentOperator, lexerContext, endWordPosition);
                    lexerContext.Advance(assignmentOperatorLength);
                    continue;
                }

                auto evaluatingSymbolType = TestForEvaluativeSymbol(rawContent, lexerContext.AbsolutePosition);
                if (evaluatingSymbolType != EvaluativeSymbolTypes::NotAnEvaluativeSymbol)
                {
                    auto symbolTypeLength = static_cast<int32_t>(evaluatingSymbolType);
                    auto endWordPosition = lexerContext.Clone(symbolTypeLength - 1);
                    AddLexerContent(lexerContent, RawEntryType::EvaluativeOperators, lexerContext, endWordPosition);
                    lexerContext.Advance(symbolTypeLength);
                    continue;
                }

                auto punctuateType = TestForStatementPunctuatesAndWildcards(currentCharacter);
                if (punctuateType != RawEntryType::Unknown)
                {
                    AddLexerContent(lexerContent, punctuateType, lexerContext);                    
                    lexerContext.Advance(1);
                    continue;
                }
                                                
                if (IsArithmeticOperator(currentCharacter))
                {
                    auto operatorLength = IsArithmeticShiftOperator(currentCharacter, nextCharacter) ? 2 : 1;
                    auto endWordPosition = lexerContext.Clone(operatorLength - 1);
                    AddLexerContent(lexerContent, RawEntryType::ArithmeticOperator, lexerContext, endWordPosition);
                    lexerContext.Advance(operatorLength);
                    continue;
                }

                auto scopeDepthPunctuates = TestForScopeDepthPunctuates(currentCharacter);
				if (scopeDepthPunctuates != RawEntryType::Unknown)
				{
                    auto scopeDepthIncrease = (scopeDepthPunctuates == RawEntryType::BracketOpen) ? 1 : (scopeDepthPunctuates == RawEntryType::BracketClose) ? -1 : 0; 
                    auto parenthesisDepthIncrease = (scopeDepthPunctuates == RawEntryType::ParenthesisOpen) ? 1 : (scopeDepthPunctuates == RawEntryType::ParenthesisClose) ? -1 : 0;
                    AddLexerContent(lexerContent, scopeDepthPunctuates, lexerContext);
                    lexerContext.SafeAddScopeDepth(scopeDepthIncrease);
                    lexerContext.SafeAddParenthesisDepth(parenthesisDepthIncrease);
                    lexerContext.Advance(1);
                    continue;
				}                

                if (CanCharBeStartOfWord(currentCharacter))
                {
                    blockStartPosition = lexerContext;
                    lexerContext.Advance(1, LexerStates::InWord);
                    continue;
                }

            	// If all else failed, this character is unknown to us.
                AddLexerContent(lexerContent, RawEntryType::Unknown, lexerContext);
                lexerContext.Advance(1, LexerStates::InWord);
                break;
            }

            case LexerStates::InComment:
            {
                auto commentClosureDetected = previousCharacter == '*' && currentCharacter == '/';
                auto endCommentPosition = lexerContext.Clone(endOfStreamReached ? -1 : 0);
                             
				if (endOfStreamReached || commentClosureDetected)
				{
                    AddLexerContent(lexerContent, RawEntryType::Comment, blockStartPosition, endCommentPosition);
				}

                auto updatedMachineState = commentClosureDetected ? LexerStates::Default : LexerStates::InComment;

                if (isLineFeed)
                {
                    lexerContext.AdvanceByLineFeed(updatedMachineState);
                }
                else
                {
                    lexerContext.Advance(1, updatedMachineState);
                }
                break;
            }

            case LexerStates::InString:
            {
                auto stringClosureDetected = IsDoubleQuotation(currentCharacter) && previousCharacter != '\\';                                
				auto newLineBreaksString = isLineFeed && previousCharacter != '\\';
                auto endOfStringPosition = lexerContext.Clone(endOfStreamReached || newLineBreaksString ? -1 : 0);

				if (endOfStreamReached || stringClosureDetected || newLineBreaksString)
				{                    
                    AddLexerContent(lexerContent, RawEntryType::String, blockStartPosition, endOfStringPosition);
				}

                auto updatedMachineState = newLineBreaksString || stringClosureDetected ? LexerStates::Default : LexerStates::InString;
                if (isLineFeed)
                {
                    lexerContext.AdvanceByLineFeed(updatedMachineState);
                }
                else
                {
                    lexerContext.Advance(1, updatedMachineState);
                }
                break;
            }

            case LexerStates::InNumber:
            case LexerStates::InWord:
            {
                bool continueLexing = false;
                auto typeOfRawEntry = (lexerContext.State == LexerStates::InNumber) ? RawEntryType::Number : RawEntryType::String;

				if (typeOfRawEntry == RawEntryType::Number)
				{
                    continueLexing = IsNumberPostfix(currentCharacter) || IsNumberPrefix(currentCharacter) || IsNumberOrHex(currentCharacter);
				}
                else
                {
                    auto continueAsCharacterIsOK = !endOfStreamReached && CanCharBeWithinWord(currentCharacter);
                    auto continueAsCharacterIsEscaped = (previousCharacter == '\\') && !endOfStreamReached && (!IsWhitespace(currentCharacter));
                    continueLexing = continueAsCharacterIsOK || continueAsCharacterIsEscaped;                
                }
	
                if (continueLexing)
                {
                    lexerContext.Advance(1);
                    continue;
                }
                                
                auto endOfWordPosition = lexerContext.Clone(-1); // Word has already ended in the previous position.
                AddLexerContent(lexerContent, typeOfRawEntry, blockStartPosition, endOfWordPosition);

                // Note: this character needs to be reprocessed, hence do not advance our position.
                // Note: Since this character will be reprocessed, we do not need to worry about LineFeed either.
                lexerContext.UpdatedState(LexerStates::Default); 
                break;
            }

            default:
                throw CLexerException(PreliminaryParsingExceptionType::InternalParserError, "Lexer state was not recognized.");            
        }

    } while (!endOfStreamReached);

    std::string extractedFileName;
    std::vector<std::string> splitString;

#ifdef COMPILING_FOR_WINDOWS
    splitString = VisualLinkerScript::StringSplit(absoluteFilePath, '\\');
#elif COMPILING_FOR_UNIX_BASED
    auto splitString = VisualLinkerScript::StringSplit(absoluteFilePath, '/');
#else
#error Either 'COMPILING_FOR_WINDOWS' need to be set or 'COMPILING_FOR_UNIX_BASED'
#endif


    // Generate result
    QString qRawContent = QString::fromStdString(rawContent);
    QString debugOutput = "";
    for (auto entry : lexerContent)
    {
        QString translatedType;

        switch (entry.EntryType())
        {
        case RawEntryType::Word:
            translatedType = "Word";
            break;

        case RawEntryType::ArithmeticOperator:
            translatedType = "ArithmeticOperator";
            break;

        case RawEntryType::Semicolon:
            translatedType = "Semicolon";
            break;

        case RawEntryType::Comma:
            translatedType = "Comma";
            break;

        case RawEntryType::AssignmentOperator:
            translatedType = "Assignment";
            break;

        case RawEntryType::Number:
            translatedType = "Number";
            break;

        case RawEntryType::String:
            translatedType = "String";
            break;

        case RawEntryType::Comment:
            translatedType = "Comment";
            break;

        case RawEntryType::ParenthesisOpen:
            translatedType = "ParenthesisOpen";
            break;

        case RawEntryType::ParenthesisClose:
            translatedType = "ParenthesisClose";
            break;

        case RawEntryType::BracketOpen:
            translatedType = "BracketOpen";
            break;

        case RawEntryType::BracketClose:
            translatedType = "BracketClose";
            break;

        case RawEntryType::Unknown:
            translatedType = "Unknown";
            break;

		case RawEntryType::NotPresent:
            translatedType = "NotPresent";
			break;
		case RawEntryType::EvaluativeOperators:
            translatedType = "EvaluativeOperators";
			break;

		case RawEntryType::Wildcard:
            translatedType = "Wildcard";
			break;

		case RawEntryType::Colon:
            translatedType = "Colon";
			break;

		case RawEntryType::QuestionMark:
            translatedType = "QuestionMark";
			break;

        default:
            translatedType = "ERROR";
            break;
        }

        /*
        auto formattedOutput = QString("Type: %1\t\t, From line %2\t index %3\t to %4\t index %5\t, BytePos: %6\t (%7\t bytes). P-Depth: %8\t, S-Depth: %9\t Content: '%10'")
            .arg(translatedType)
            .arg(entry.StartLineNumber())
            .arg(entry.StartIndexInLine())
            .arg(entry.EndLineNumber())
            .arg(entry.EndIndexInLine())
            .arg(entry.StartPosition())
            .arg(entry.Length())
            .arg(entry.ParenthesisDepth())
            .arg(entry.ScopeDepth())
            .arg(qRawContent.mid(entry.StartPosition(), entry.Length()));

        qDebug() << formattedOutput;
        */
    }

    extractedFileName = splitString.back();
    return std::make_shared<CRawFile>(std::move(rawContent),
                                      extractedFileName,
                                      absoluteFilePath,
                                      std::move(lineIndentationMap),
                                      std::move(lexerContent));
}
