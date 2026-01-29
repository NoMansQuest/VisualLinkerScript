// Scintilla source code edit control
/** @file LexLinkerScript.cxx
 ** Lexer for GNU Linker-Script files.
 **
 ** First working version contributed by Nasser GHOSEIRI <nasser.ghoseiri@gmail.com> on October 23, 2022. This file was created
 ** as part of the VisualLinkerScript project.
 **/


#include <stdlib.h>
#include <string.h>
#include <exception>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>
#include <stdexcept>

#include "Components/QScintilla/scintilla/include/ILexer.h"
#include "Components/QScintilla/scintilla/include/Scintilla.h"
#include "Components/QScintilla/scintilla/include/SciLexer.h"

#include "Components/QScintilla/scintilla/lexlib/WordList.h"
#include "Components/QScintilla/scintilla/lexlib/LexAccessor.h"
#include "Components/QScintilla/scintilla/lexlib/Accessor.h"
#include "Components/QScintilla/scintilla/lexlib/StyleContext.h"
#include "Components/QScintilla/scintilla/lexlib/CharacterSet.h"
#include "Components/QScintilla/scintilla/lexlib/LexerModule.h"

#define UNREFERENCED_PARAMETER(P) (P)

using namespace Scintilla;

/// @brief Anonymous namespace, used to avoid naming conflicts
namespace LinkerScript
{
    /// @brief Type of numbers we Support
    enum class NumberType : int
    {
        Decimal = 0,    // Base-10, Starts with any 1-9 digit.
        Octal,          // Base-8, starts with leading '0'
        Hexadecimal,    // Base-16, starts with '0x'
        Binary,         // Base-2, starts with 0b
        Unidentified    // Unidentified type (or invalid)
    };

    /// @brief These words are considered as "Reserved", and have their own proper coloring
    static const char * linkerScriptReservedWords[] = {
        "MEMORY", "SECTIONS", "PHDRS", "VERSION", "OVERLAY", "NOCROSSREF", "AT", "PT_NULL"
        "PT_LOAD", "PT_DYNAMIC", "PT_INTERP", "PT_NOTE", "PT_SHLIB", "PT_PHDR", "FORCE_COMMON_ALLOCATION",
        "ONLY_IF_RW", "ONLY_IF_RO", "/DISCARD/", "MAXPAGESIZE", "SIZEOF_HEADERS",
        "INCLUDE", "INPUT", "GROUP", "OUTPUT", "OUTPUT_ARCH", "OUTPUT_FORMAT", "SEARCH_DIR", 0
    };

    /// @brief These are linker-script Functions, and are colored accordingly
    static const char * linkerScriptFunctions[] = {
        "ENTRY", "ALIGN", "ORIGIN", "LENGTH", "PROVIDE", "PROVIDE_HIDDEN", "SEGMENT_START", "KEEP", "CONSTANT",
        "SORT", "SORT_BY_ALIGNMENT", "SORT_BY_NAME", "SORT_BY_INIT_PRIORITY", "SORT_NONE",
        "EXCLUDE_FILE", "BYTE", "SHORT", "LONG", "QUAD", "SQUAD", "FLOAT", "NOFLOAT",
        "FORCE_COMMON_ALLOCATION", "STARTUP", "TARGET", "LOADADDR", "DATA_SEGMENT_ALIGN",
        "DATA_SEGMENT_RELRO_END", 0
    };

    /// @brief Is this word present in the given list?
    /// @param startPosition Position of the first character of the keyword
    /// @param length Length of the keyword
    /// @param listToCheckAgainst List of words to check against
    /// @param styler Needed to access content
    /// @return 'True' if a keyword (and needs to be styled as such), 'False' otherwise...
    bool IsKeywordInList(Sci_PositionU startPosition, size_t length, const char** listToCheckAgainst, Accessor& styler)
    {        
        auto match = false;
        for (auto reservedWordIndex = 0; listToCheckAgainst[reservedWordIndex] != 0; reservedWordIndex++)
        {            
            auto keywordToCheckAgainst = listToCheckAgainst[reservedWordIndex];
            auto keywordLength = strlen(keywordToCheckAgainst);

            if (keywordLength != length)
            {
                continue;
            }

            auto mismatched = false;
            for (auto index = 0; index < length; index++)
            {
                if (styler.SafeGetCharAt(startPosition + index) != keywordToCheckAgainst[index])
                {
                    mismatched = true;
                    break;
                }
            }       

            if (!mismatched)             
            {
                match = true;
                break;
            }            
        }
        return match;
    }

    /// @brief Determines if this character can be the first letter of a word
    /// @param input Character to check
    /// @return True if the character can be the first letter of a word (i.e. A-Z, a-z, '_')
    bool CanBeStartOfWord(char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||               
               (input == '_') ||                           
               (input == '.');
    }

    /// @brief If true, then the character can be considered as part of a word
    /// @param input Character to check
    /// @return True if the character can be considered as part of word (i.e. A-Z, a-z, 0-9 and '_')
    bool CanBeWithinWord(char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||
               ((input >= '0') && (input <= '9')) ||               
               (input == '_') ||
               (input == '-') || (input == '+') ||  // Yes, weirdly enough these are allowed in LinkerScript symbol names!
               (input == '.');
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

    /// @brief  Reports back the type of number in the given string ( @see NumeberType ). It also checks
    ///         if the pointer is valid.
    /// @param  startPosition Index of the first character to check
    /// @param  length Length of the string. We don't use strlen here for optimization
    /// @param  styler Styler. We use it to obtain characters mostly
    /// @return Type of the number as described in @see NumeberType
    LinkerScript::NumberType SafeDetectNumberType(Sci_PositionU startPosition, size_t length, Accessor &styler)
    {
        auto charAt0 = styler.SafeGetCharAt(startPosition);

        if ((charAt0 == '0') && (length > 1))
        {
            auto charAt1 = styler.SafeGetCharAt(startPosition+1);

            if ((charAt1 == 'x') || (charAt1 == 'X'))
            {
                return NumberType::Hexadecimal;
            }
            else if ((charAt1 == 'd') || (charAt1 == 'D'))
            {
                return NumberType::Decimal;
            }
            else if ((charAt1 == 'b') || (charAt1 == 'B'))
            {
                return NumberType::Binary;
            }
            else if (IsADigit(charAt1))
            {
                return NumberType::Octal;
            }
            else
            {
                return NumberType::Unidentified;
            }
        }

        return (IsADigit(charAt0) ? NumberType::Decimal : NumberType::Unidentified);
    }

    /// @brief Checks to see if the provided string inbetween these positions represent a valid number.
    /// @param startPosition Position to start scanning
    /// @param length Length of the number to check. We don't use 'strlen' for performance reasons.
    /// @param styler Needed to access content
    /// @return Type of the number (Unidentified if not a valid number)
    NumberType CheckNumber(Sci_PositionU startPosition, size_t length, Accessor &styler)
    {
        auto numberType = SafeDetectNumberType(startPosition, length, styler);
        auto baseToCheck = 0;
        auto validationStartIndex = startPosition;
        auto lastIndexToCheck = startPosition + length - 1;

        switch (numberType)
        {
            case NumberType::Decimal:
            {
                // On 'Decimal' numbers support postfixes, which could be either K or M.
                auto lastChar = styler.SafeGetCharAt(startPosition + length - 1);
                if (!IsADigit(lastChar) & (length > 1)) // Only lengths greater than two are checked for this
                {
                    if ((lastChar == 'K') || (lastChar == 'M'))                       
                    {                                               
                        lastIndexToCheck = startPosition + length - 2; // This is so that the we don't include the last char in the verification loop.
                    }
                    else
                    {    
                        return NumberType::Unidentified;
                    }                     
                }                               

                baseToCheck = 10;
                validationStartIndex = 0;
                break;
            }

            case NumberType::Octal:
            {
                baseToCheck = 8;
                validationStartIndex = 1;
                break;    
            }

            case NumberType::Hexadecimal:
            {
                baseToCheck = 10;
                validationStartIndex = 2;
                break; 
            }

            case NumberType::Binary:
            {
                baseToCheck = 2;
                validationStartIndex = 2;
                break;
            }

            default:
            {
                return numberType;
            }
        }

        for (auto hover = validationStartIndex; hover <= lastIndexToCheck; hover++)
        {
            if (!IsADigit(styler.SafeGetCharAt(hover), baseToCheck))                    
            {
                return numberType; 
            }
        }

        return NumberType::Unidentified;  
    }


    /// @brief Goes back in the text to find the position of the first digit in the number
    /// @param startingPosition Where to start from
    /// @param styler Needed for content access
    /// @return Position of the first character of the number
    Sci_PositionU FindStartOfNumber(Sci_PositionU startingPosition, Accessor& styler)    
    {
        auto processingPosition = startingPosition;
        while (startingPosition >= 0)
        {
            auto currentChar = styler.SafeGetCharAt(processingPosition);
            if (IsADigit(currentChar, 16) || LinkerScript::IsNumberPrefix(currentChar))
            {
                processingPosition--;
                continue;
            }

            break;
        }

        return processingPosition + 1;
    }

    /// @brief Goes back in the text to find the position of the first digit in the number
    /// @param startingPosition Where to start from
    /// @param styler Needed for content access
    /// @return Position of the first character of the number
    Sci_PositionU FindStartOfWord(Sci_PositionU startingPosition, Accessor& styler)    
    {        
        auto processingPosition = startingPosition;
        while (startingPosition >= 0)
        {
            auto currentChar = styler.SafeGetCharAt(processingPosition);
            if (CanBeWithinWord(currentChar))
            {
                processingPosition--;
                continue;
            }

            break;
        }

        processingPosition += 1; 
        return (CanBeStartOfWord(styler.SafeGetCharAt(processingPosition)) ?
                processingPosition : 
                processingPosition + 1);
    }

    /// @brief Goes back in the text to find where the comment actually started
    /// @param startingPosition Where to start from
    /// @param styler Needed for content access
    /// @return Position of the first character of the comment, which would be a '/' followed by a '*'
    Sci_PositionU FindStartOfComment(Sci_PositionU startingPosition, Accessor& styler) 
    {        
        auto nextChar = styler.SafeGetCharAt(startingPosition+1);
        auto previousChar = styler.SafeGetCharAt(startingPosition-1);

        // Check if we're actually breaking the comment header
        if ((previousChar == '/') && (nextChar == '*'))
        {
            return startingPosition - 1;
        }

        // Go back till we find a '*' preceded by a '/'
        auto starFound = false;        
        auto hoveringPosition = startingPosition;

        while (hoveringPosition >= 0)
        {
            auto currentChar = styler.SafeGetCharAt(hoveringPosition);
            if (((starFound == true) && (currentChar == '/')) || 
                (hoveringPosition == 0))
            {
                break;
            }
            
            starFound = (currentChar == '*');
            hoveringPosition--;            
        }

        return hoveringPosition;
    }        
}

static void ColouriseLinkerScriptDoc(Sci_PositionU startPos, Sci_Position length, int initStyle, WordList *keywordLists[], Accessor &styler)
{
    UNREFERENCED_PARAMETER(keywordLists);

    int state = SCE_LINKERSCRIPT_DEFAULT;
    constexpr Sci_PositionU processedStartPosition = 0;        
    const Sci_PositionU lastPositionToCheck = styler.Length() - 1;
    auto nextChar = styler.SafeGetCharAt(processedStartPosition);

    // Go through all provided text segment using the handwritten state machine shown below
    styler.StartAt(processedStartPosition);
    styler.StartSegment(processedStartPosition);

    Sci_PositionU unknownObjectStartIndex = 0;   
    Sci_PositionU index = processedStartPosition;
    auto keepScanning = true;

    while (keepScanning)
    {
        char currentChar = nextChar;
        nextChar = styler.SafeGetCharAt(index + 1);

        if (styler.IsLeadByte(currentChar)) 
        {
            nextChar = styler.SafeGetCharAt(index + 2);
            index += 2;
            continue;
        }

        switch(state) 
        {         
           case SCE_LINKERSCRIPT_DEFAULT:
           {
               // Check if we need to abort 
               if (index > lastPositionToCheck) 
               {
                    keepScanning = false;
                    break;
               }

               if (LinkerScript::IsOperator(currentChar))
               {
                    // The only exception here is slash, which may indicate either start of comments or a divide operator
                    if ((currentChar == '/') && (nextChar == '*'))
                    {
                        nextChar = styler.SafeGetCharAt(index+2);
                        index += 2;
                        state = SCE_LINKERSCRIPT_COMMENTS;
                        continue;                       
                    }
                    
                    styler.ColourTo(index, SCE_LINKERSCRIPT_OPERATORS);
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;
               }

               if (currentChar == '"')               
               {
                    styler.ColourTo(index, SCE_LINKERSCRIPT_STRING_START);
                    state = SCE_LINKERSCRIPT_STRING_CONTENT;
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;
               }

               if (LinkerScript::CanBeStartOfWord(currentChar))
               {    
                    // Check special-case
                    if (LinkerScript::CanBeWithinWord(nextChar))
                    {                        
                        unknownObjectStartIndex = index;
                        state = SCE_LINKERSCRIPT_UNKNOWN_WORD;
                        nextChar = currentChar;
                        continue;
                    }
                    else if (currentChar == '.')
                    {
                        styler.ColourTo(index, SCE_LINKERSCRIPT_LOCATION_COUNTER);
                        state = SCE_LINKERSCRIPT_DEFAULT;
                        nextChar = styler.SafeGetCharAt(++index);
                        continue;
                    }

                    // This is a single-letter symbol then...
                    styler.ColourTo(index, SCE_LINKERSCRIPT_DEFAULT);
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;                    
               }

               if (IsADigit(currentChar))
               {
                    unknownObjectStartIndex = index;
                    nextChar = styler.SafeGetCharAt(++index);
                    state = SCE_LINKERSCRIPT_UNKNOWN_NUMBER;
                    continue;    
               }

               if ((currentChar == '{') || (currentChar == '}'))
               {
                    styler.ColourTo(index, SCE_LINKERSCRIPT_CURLY_BRACKETS);
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;
               }

               if ((currentChar == '(') || (currentChar == ')'))
               {
                    styler.ColourTo(index, SCE_LINKERSCRIPT_PARENTHESIS);
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;
               }
                              
               styler.ColourTo(index++, SCE_LINKERSCRIPT_DEFAULT);
               nextChar = styler.SafeGetCharAt(index);
               continue;
           }          

           case SCE_LINKERSCRIPT_UNKNOWN_NUMBER:
           {
               if ((IsADigit(currentChar, 16) || LinkerScript::IsNumberPrefix(currentChar)) &&
                   (index <= lastPositionToCheck))
               {
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;
               }

               // OK, We have the position of the last character in the number. 
               const auto lastIndexToConsider = LinkerScript::IsNumberPostfix(currentChar) ? index : index - 1;

               switch (LinkerScript::CheckNumber(unknownObjectStartIndex, lastIndexToConsider - unknownObjectStartIndex + 1, styler)) 
               {
                case LinkerScript::NumberType::Decimal:
                    styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_DECIMAL_NUMBER);
                    break;

                case LinkerScript::NumberType::Octal:
                    styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_OCTAL_NUMBER);
                    break;    

                case LinkerScript::NumberType::Hexadecimal:
                    styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_HEXADECIMAL_NUMBER);
                    break; 

                case LinkerScript::NumberType::Binary:
                    styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_BINARY_NUMBER);
                    break;

                default:
                    styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_UNKNOWN_NUMBER);
                    break;
               }

               // We now need to reinvestigate the current character
               state = SCE_LINKERSCRIPT_DEFAULT;
               nextChar = styler.SafeGetCharAt(lastIndexToConsider + 1);
               index = lastIndexToConsider + 1;
               continue;
           }

           case SCE_LINKERSCRIPT_UNKNOWN_WORD:
           {   
               if (LinkerScript::CanBeWithinWord(currentChar) && (index <= lastPositionToCheck))
               {
                   nextChar = styler.SafeGetCharAt(++index);
                   continue;
               }

               state = SCE_LINKERSCRIPT_DEFAULT; 
               auto lastIndexToConsider = index - 1;
               auto unknownWordLength = lastIndexToConsider - unknownObjectStartIndex + 1;

               auto previousChar = styler.SafeGetCharAt(index - 1);

               // Check for word type
               if (LinkerScript::IsKeywordInList(unknownObjectStartIndex, 
                                                 unknownWordLength,
                                                 LinkerScript::linkerScriptFunctions,
                                                 styler ))
               {                   
                   styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_FUNCTION);
               }
               else if (LinkerScript::IsKeywordInList(unknownObjectStartIndex, 
                                                 unknownWordLength,
                                                 LinkerScript::linkerScriptReservedWords,
                                                 styler ))
               {
                   styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_RESERVED);
               }
               else if ((unknownWordLength == 1) && (previousChar == '.'))
               {
                   styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_LOCATION_COUNTER);
               }
               else
               {
                   styler.ColourTo(lastIndexToConsider, SCE_LINKERSCRIPT_DEFAULT);					
               }

               // We now need to reinvestigate the current character               
               state = SCE_LINKERSCRIPT_DEFAULT;
               nextChar = currentChar;               
               continue; 
           }

           case SCE_LINKERSCRIPT_PARENTHESIS:
           case SCE_LINKERSCRIPT_CURLY_BRACKETS:
           case SCE_LINKERSCRIPT_LOCATION_COUNTER: 
           case SCE_LINKERSCRIPT_OPERATORS:          
           {
                nextChar = styler.SafeGetCharAt(--index);
                state = SCE_LINKERSCRIPT_DEFAULT;
                break;
           }

           case SCE_LINKERSCRIPT_COMMENTS:
           {
               // Check if we need to abort 
               if (index > lastPositionToCheck) 
               {
                    keepScanning = false;
                    break;
               }

               if ((currentChar == '/') && (styler.SafeGetCharAt(index - 1) == '*'))
               {
                    state = SCE_LINKERSCRIPT_DEFAULT;                    
               }

               styler.ColourTo(index, SCE_LINKERSCRIPT_COMMENTS);
               nextChar = styler.SafeGetCharAt(++index);
               break;
           }

           case SCE_LINKERSCRIPT_STRING_CONTENT:
           {
               // Check if we need to abort 
               if (index > lastPositionToCheck) 
               {
                    keepScanning = false;
                    break;
               }

               if ((currentChar == '"') && (styler.SafeGetCharAt(index - 1) != '\\'))
               {
                    styler.ColourTo(index - 1, SCE_LINKERSCRIPT_STRING_CONTENT);
                    styler.ColourTo(index, SCE_LINKERSCRIPT_STRING_END);
                    state = SCE_LINKERSCRIPT_DEFAULT;
               }
               else
               {
                    styler.ColourTo(index, SCE_LINKERSCRIPT_STRING_CONTENT);                    
               } 

               nextChar = styler.SafeGetCharAt(++index);
               break;
           }

           default:
           {
               // This is an error indeed. We don't throw any exceptions at this point in time
               // until the pattern is established...
               return;
           } 
        }
    }
}

static void FoldLinkerScriptDoc(Sci_PositionU startingPosition, Sci_Position length, int initStyle, WordList *keywordLists[], Accessor &styler)
{
    UNREFERENCED_PARAMETER(keywordLists);

    enum class FolderStateEnum
    {
        Default,
        InComment,
        InString,
    };

    const auto endingPosition = 0 + styler.Length();
    auto currentLine = styler.GetLine(0);
    auto currentFolderState = FolderStateEnum::Default;
    auto previousCharacter = ' '; // We don't care what it truly had been
    int levelPrev = styler.LevelAt(currentLine);
    int levelCurrent = levelPrev;

    for (unsigned int hoveringIndex = 0; hoveringIndex < endingPosition; hoveringIndex++) 
    {
        const auto nextCharacter = styler.SafeGetCharAt(hoveringIndex + 1);	    
	    const char currentCharacter = styler.SafeGetCharAt(hoveringIndex);
        const auto newLineJumpToNextCharacter = (currentCharacter == '\r' && nextCharacter == '\n');
        const auto atEndOfLine = newLineJumpToNextCharacter || (currentCharacter == '\n');

        switch (currentFolderState)
        {
	        case FolderStateEnum::Default:
	        {
                if (currentCharacter == '{')
                {
                    levelCurrent++;
                }

                if (currentCharacter == '}')
                {
                    levelCurrent--;
                }

				if ((currentCharacter == '/') && (nextCharacter == '*'))
				{
                    levelCurrent++;
                    currentFolderState = FolderStateEnum::InComment;
				}
				
                if ((currentCharacter == '"'))
                {
                    currentFolderState = FolderStateEnum::InString;
                }

				break;
	        }

	        case FolderStateEnum::InComment:
	        {
				if (currentCharacter == '*' && nextCharacter == '/')
				{
                    levelCurrent--;
                    currentFolderState = FolderStateEnum::Default;
				}				
                break;
	        }

	        case FolderStateEnum::InString:
	        {
				if ((currentCharacter == '"') && (previousCharacter != '\\'))
				{
                    currentFolderState = FolderStateEnum::Default;
				}
                break;
	        }

            /*
             * The default was left here to catch potential exceptions triggered
             * by a potential future enum entry not referenced in the switch statement.
             */
	        default:
	            throw std::runtime_error("Unrecognized enum state in 'FolderStateEnum'");            
        }

        if (atEndOfLine || (hoveringIndex == (endingPosition - 1)))
        {
            int lev = levelPrev;
            if (levelCurrent > levelPrev)
            {
                lev |= SC_FOLDLEVELHEADERFLAG;
            }

            if (lev != styler.LevelAt(currentLine))
            {
                styler.SetLevel(currentLine, lev);
            }
            
            currentLine++;            
            levelPrev = levelCurrent;
        }

        if (newLineJumpToNextCharacter)
        {            
            previousCharacter = nextCharacter;
            hoveringIndex++;
        }
        else
        {            
            previousCharacter = currentCharacter;
        }
        
    }
}
LexerModule lmLinkerScript(SCLEX_LINKERSCRIPT, ColouriseLinkerScriptDoc, "linkerScript", FoldLinkerScriptDoc);

