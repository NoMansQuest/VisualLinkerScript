// Scintilla source code edit control
/** @file LexLinkerScript.cxx
 ** Lexer for GNU Linker-Script files.
 **
 ** First working version contributed by Nasser GHOSEIRI <nasser.ghoseiri@gmail.com> on October 23, 2022. This file was created
 ** as part of the Visual Linker Script project.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include "../include/ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "../lexlib/StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

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
        "INCLUDE", "INPUT", "GROUP", "OUTPUT", "OUTPUT_ARCH", "OUTPUT_FORMAT", "SEARCH_DIR", 0
    };

    /// @brief These are linker-script Functions, and are colored accordingly
    static const char * linkerScriptFunctions[] = {
        "ENTRY", "ALIGN", "PROVIDE", "KEEP", "BYTE", "SHORT", "LONG", "QUAD", "SQUAD", "FLOAT", "NOFLOAT",
        "FORCE_COMMON_ALLOCATION", "STARTUP", "TARGET", "LOADADDR",
        0
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
            break;
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
    int state = (startPos == 0) ? SCE_LINKERSCRIPT_DEFAULT : initStyle;    
    auto processedStartPosition = startPos;    

    // Adjust state and position based on special cases  
    switch (state)
    {   
        case SCE_LINKERSCRIPT_RESERVED:
        case SCE_LINKERSCRIPT_FUNCTION:
            processedStartPosition = LinkerScript::FindStartOfWord(startPos, styler);
            state = SCE_LINKERSCRIPT_DEFAULT;
            break;

        case SCE_LINKERSCRIPT_HEXADECIMAL_NUMBER:
        case SCE_LINKERSCRIPT_DECIMAL_NUMBER:
        case SCE_LINKERSCRIPT_OCTAL_NUMBER:
        case SCE_LINKERSCRIPT_BINARY_NUMBER:
            processedStartPosition = LinkerScript::FindStartOfNumber(startPos, styler);
            state = SCE_LINKERSCRIPT_DEFAULT; 
            break;

        case SCE_LINKERSCRIPT_COMMENTS:
            processedStartPosition = LinkerScript::FindStartOfComment(startPos, styler);
            state = SCE_LINKERSCRIPT_DEFAULT; 
            break;

        case SCE_LINKERSCRIPT_STRING_START:
            state = SCE_LINKERSCRIPT_STRING_CONTENT;
            break;

        case SCE_LINKERSCRIPT_STRING_END:
            state = SCE_LINKERSCRIPT_DEFAULT;
            break;
    }

    // We need to know the length of the document
    auto nextChar = styler.SafeGetCharAt(processedStartPosition);
    Sci_PositionU lastPositionToCheck = processedStartPosition + length - 1;         

    // Go through all provided text segment using the hand-written state machine shown below
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
               else if ((currentChar == '{') || (currentChar == '}'))
               {
                    styler.ColourTo(index, SCE_LINKERSCRIPT_CURLY_BRACKETS);
                    nextChar = styler.SafeGetCharAt(++index);
                    continue;
               }
               else if ((currentChar == '(') || (currentChar == ')'))
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
               auto lastIndexToConsider = LinkerScript::IsNumberPostfix(currentChar) ? index : index - 1;
               auto numberType = LinkerScript::CheckNumber(unknownObjectStartIndex, lastIndexToConsider - unknownObjectStartIndex + 1, styler);

               switch (numberType) 
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

LexerModule lmLinkerScript(SCLEX_LINKERSCRIPT, ColouriseLinkerScriptDoc, "linkerScript", 0);

