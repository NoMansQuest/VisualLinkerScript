// Scintilla source code edit control
/** @file LexLinkerScript.cxx
 ** Lexer for GNU Linker-Script files.
 **
 ** First working version contributed by Nasser GHOSEIRI <nasser.ghoseiri@gmail.com> on October 23, 2022. This file was created
 ** as part of the Visual Linker Script project.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

/*
 * 
   Colors:
   =========================================================
   Comment Green: 87, 166, 74                   = 0xff57A64A
   Parameter Field/Class Green : 78,201,176     = 0xff4EC9B0
   Enum Field Green: 181,206,168                = 0xffB5CEA8
   Text Beige: 214,157,133                      = 0xffD69D85
   Violet: 189, 99, 197                         = 0xffBD63C5
   Darker Code Gray: 127, 127, 127              = 0xff7F7F7F
   Lighter Code Gray: 155,155,155               = 0xff9B9B9B
   Code White: 200, 200, 200                    = 0xffC8C8C8
   Enum Blue: 186,156, 214                      = 0xff569CD6
   Bracket Yellow Code: 255,215,4               = 0xffFFD704

   States:
   =========================================================
   SCE_LINKERSCRIPT_DEFAULT 
   SCE_LINKERSCRIPT_RESERVED 
   SCE_LINKERSCRIPT_FUNCTION 
   SCE_LINKERSCRIPT_CURLY_BRACKETS 
   SCE_LINKERSCRIPT_PARENTHESIS 
   SCE_LINKERSCRIPT_OPERATORS 
   SCE_LINKERSCRIPT_COMMENTS 
   SCE_LINKERSCRIPT_STRINGS 

 */

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


#define _LINKERSCRIPT_MAXIMUM_NUMBER_LENGTH 128

using namespace Scintilla;

/// @brief These words are considered as "Reserved", and have their own proper coloring
static const char * const linkerScriptReservedWords[] = {
    "MEMORY", "SECTIONS", "PHDRS", "VERSION", "OVERLAY", "NOCROSSREF", "AT", "PT_NULL"
    "PT_LOAD", "PT_DYNAMIC", "PT_INTERP", "PT_NOTE", "PT_SHLIB", "PT_PHDR", "FORCE_COMMON_ALLOCATION",
    "INCLUDE", "INPUT", "GROUP", "OUTPUT", "OUTPUT_ARCH", "OUTPUT_FORMAT", "SEARCH_DIR", 0
};

/// @brief These are linker-script Functions, and are colored accordingly
static const char * const linkerScriptFunctions[]] = {
    "ALIGN", "PROVIDE", "KEEP", "BYTE", "SHORT", "LONG", "QUAD", "SQUAD", "FLOAT", "NOFLOAT",
    "FORCE_COMMON_ALLOCATION", "STARTUP", "TARGET",
    0
};

/// @brief Anonymous namespace, used to avoid naming conflicts
namespace
{
    /// @brief Type of numbers we Support
    enum class NumberType
    {
        Decimal,        // Base-10, Starts with any 1-9 digit.
        Octal,          // Base-8, starts with leading '0'
        Hexadecimal,    // Base-16, starts with '0x'
        Binary,         // Base-2, starts with 0b
        Unidentified    // Unidentified type (or invalid)
    }

    /// @brief Determines if this character can be the first letter of a word
    /// @param input Character to check
    /// @return True if the character can be the first letter of a word (i.e. A-Z, a-z, '_')
    bool canBeStartOfWork(char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||               
               (input == '_');
    }

    /// @brief If true, then the character can be considered as part of a word
    /// @param input Character to check
    /// @return True if the character can be considered as part of word (i.e. A-Z, a-z, 0-9 and '_')
    bool canBeWordContent(char input)
    {
        return ((input >= 'A') && (input <= 'Z')) ||
               ((input >= 'a') && (input <= 'z')) ||
               ((input >= '0') && (input <= '9')) ||
               (input == '_');
    }

    /// @brief Checks to see if the provided string inbetween these positions represent a valid number.
    /// @param numberToCheck String containing the number to check
    /// @param length Length of the number to check. We don't use 'strlen' for performance reasons.
    /// @return 
    bool isValidNumber(const char* stringToCheck, int length)
    {
        auto numberType = safeDetectNumberType(stringToCheck);
        auto baseToCheck = 10;
        auto validationStartIndex = 0;
        auto lastIndexToCheck = length - 1;

        switch (numberType)
        {
            case NumberType::Decimal:
                // On 'Decimal' numbers support postfixs, which could be either K or M.
                auto lastChar = stringToCheck[length - 1];
                if (!IsADigit(lastChar)) 
                {
                    if ((lastChar == 'K') || (lastChar == 'M'))                       
                    {                            
                        lastIndexToCheck = length - 2; // This is so that the we don't include the last char in the verification loop.
                    }
                    else
                    {    
                        return false;
                    }                     
                }                               

                baseToCheck = 10;
                validationStartIndex = 0
                break;

            case NumberType::Octal:
                baseToCheck = 8;
                validationStartIndex = 1
                break;    

            case NumberType::Hexadecimal:
                baseToCheck = 10;
                validationStartIndex = 2
                break; 

            case NumberType::Binary:
                baseToCheck = 2;
                validationStartIndex = 2;
                break;

            default:
                return false;
        }

        for (auto hover = validationStartIndex; hover <= lastIndexToCheck; hover++)
        {
            if (!IsADigit(stringToCheck[hover], baseToCheck))                    
            {
                return false; 
            }
        }

        return true;  
    }


    /// @brief  Reports back the type of number in the given string ( @see NumeberType ). It also checks
    ///         if the pointer is valid.
    /// @param  numberToCheck String containing the number to be checked.
    /// @param  length Length of the string. We don't use strlen here for optimization
    /// @return Type of the number as described in @see NumeberType    
    NumeberType safeDetectNumberType(const char* numberToCheck, int length)
    {
        if (numberToCheck == nullptr)
        {
            return NumeberType::Unidentified;
        }

        if (numberToCheck[0] == '0')
        {
            if ((numberToCheck[1] == 'x') || (numberToCheck[1] == 'X'))
            {
                return NumberType::Hexadecimal;
            }
            else if ((numberToCheck[1] == 'd') || (numberToCheck[1] == 'D'))
            {
                return NumberType::Decimal;
            }
            else if ((numberToCheck[1] == 'b') || (numberToCheck[1] == 'B'))
            {
                return NumberType::Binary;
            }
            else if (IsADigit(numberToCheck[1]))
            {
                return NumberType::Octal;
            }
            else
            {
                return NumberType::Unidentified;   
            }
        }

        return (IsADigit(numberToCheck[0]) ? NumberType::Decimal : NumberType::Unidentified);
    }
}



static void ColouriseLinkerScriptDoc(Sci_PositionU startPos, Sci_Position length, int initStyle, WordList *keywordLists[], Accessor &styler)
{
    /*
    SCE_LINKERSCRIPT_DEFAULT 
    SCE_LINKERSCRIPT_RESERVED 
    SCE_LINKERSCRIPT_FUNCTION 
    SCE_LINKERSCRIPT_CURLY_BRACKETS 
    SCE_LINKERSCRIPT_PARENTHESIS 
    SCE_LINKERSCRIPT_OPERATORS 
    SCE_LINKERSCRIPT_COMMENTS 
    SCE_LINKERSCRIPT_STRINGS     
    */

    int state = (startPos == 0) ? SCE_LINKERSCRIPT_DEFAULT : initStyle;
    auto nextChar = styler[startPos];
    char numberValidationBuffer[_LINKERSCRIPT_MAXIMUM_NUMBER_LENGTH]; // Although unlikely a number would ever be this long!

    Sci_Position lengthDoc = startPos + length;

    // create a buffer large enough to take the largest chunk...        
    Sci_Position bufferCount = 0;

    // We have two types of keywords in this project
    WordList &reservedWords = *linkerScriptReservedWordListDesc[0];
    WordList &functions = *linkerScriptFunctionsListDesc[1];

    // go through all provided text segment
    // using the hand-written state machine shown below
    styler.StartAt(startPos);
    styler.StartSegment(startPos);
    for (auto index = startPos; index < lengthDoc; index++) 
    {
        char currentChar = nextChar;
        nextChar = styler.SafeGetCharAt(i + 1);

        if (styler.IsLeadByte(currentChar) || 
            isBackslash(currentChar)) 
        {
            nextChar = styler.SafeGetCharAt(index + 2);
            index++;
            continue;
        }

        switch(state) 
        {
            case SCE_CONF_DEFAULT:
                if( isWhiteSpace(currentChar)) 
                {
                    // whitespace is simply ignored here...
                    styler.ColourTo(i,SCE_CONF_DEFAULT);
                    break;
                } 
                else if( ch == '#' ) 
                {
                    // signals the start of a comment...
                    state = SCE_CONF_COMMENT;
                    styler.ColourTo(i,SCE_CONF_COMMENT);
                } 
                else if( ch == '.' /*|| ch == '/'*/) 
                {
                    // signals the start of a file...
                    state = SCE_CONF_EXTENSION;
                    styler.ColourTo(i,SCE_CONF_EXTENSION);
                } 
                else if( ch == '"') 
                {
                    state = SCE_CONF_STRING;
                    styler.ColourTo(i,SCE_CONF_STRING);
                } 
                else if( IsASCII(ch) && ispunct(ch) ) 
                {
                    // signals an operator...
                    // no state jump necessary for this
                    // simple case...
                    styler.ColourTo(i,SCE_CONF_OPERATOR);
                } 
                else if( IsASCII(ch) && isalpha(ch) ) 
                {
                    // signals the start of an identifier
                    bufferCount = 0;
                    buffer[bufferCount++] = static_cast<char>(tolower(ch));
                    state = SCE_CONF_IDENTIFIER;
                } 
                else if( IsASCII(ch) && isdigit(ch) ) 
                {
                    // signals the start of a number
                    bufferCount = 0;
                    buffer[bufferCount++] = ch;
                    //styler.ColourTo(i,SCE_CONF_NUMBER);
                    state = SCE_CONF_NUMBER;
                } 
                else 
                {
                    // style it the default style..
                    styler.ColourTo(i,SCE_CONF_DEFAULT);
                }
                break;

            case SCE_CONF_COMMENT:
                // if we find a newline here,
                // we simply go to default state
                // else continue to work on it...
                if( ch == '\n' || ch == '\r' ) {
                    state = SCE_CONF_DEFAULT;
                } else {
                    styler.ColourTo(i,SCE_CONF_COMMENT);
                }
                break;

            case SCE_CONF_EXTENSION:
                // if we find a non-alphanumeric char,
                // we simply go to default state
                // else we're still dealing with an extension...
                if( (IsASCII(ch) && isalnum(ch)) || (ch == '_') ||
                    (ch == '-') || (ch == '$') ||
                    (ch == '/') || (ch == '.') || (ch == '*') )
                {
                    styler.ColourTo(i,SCE_CONF_EXTENSION);
                } else {
                    state = SCE_CONF_DEFAULT;
                    chNext = styler[i--];
                }
                break;

            case SCE_CONF_STRING:
                // if we find the end of a string char, we simply go to default state
                // else we're still dealing with an string...
                if( (ch == '"' && styler.SafeGetCharAt(i-1)!='\\') || (ch == '\n') || (ch == '\r') ) {
                    state = SCE_CONF_DEFAULT;
                }
                styler.ColourTo(i,SCE_CONF_STRING);
                break;

            case SCE_CONF_IDENTIFIER:
                // stay  in CONF_IDENTIFIER state until we find a non-alphanumeric
                if( (IsASCII(ch) && isalnum(ch)) || (ch == '_') || (ch == '-') || (ch == '/') || (ch == '$') || (ch == '.') || (ch == '*')) {
                    buffer[bufferCount++] = static_cast<char>(tolower(ch));
                } else {
                    state = SCE_CONF_DEFAULT;
                    buffer[bufferCount] = '\0';

                    // check if the buffer contains a keyword, and highlight it if it is a keyword...
                    if(directives.InList(buffer)) {
                        styler.ColourTo(i-1,SCE_CONF_DIRECTIVE );
                    } else if(params.InList(buffer)) {
                        styler.ColourTo(i-1,SCE_CONF_PARAMETER );
                    } else if(strchr(buffer,'/') || strchr(buffer,'.')) {
                        styler.ColourTo(i-1,SCE_CONF_EXTENSION);
                    } else {
                        styler.ColourTo(i-1,SCE_CONF_DEFAULT);
                    }

                    // push back the faulty character
                    chNext = styler[i--];

                }
                break;

            case SCE_CONF_NUMBER:
                // stay  in CONF_NUMBER state until we find a non-numeric
                if( (IsASCII(ch) && isdigit(ch)) || ch == '.') {
                    buffer[bufferCount++] = ch;
                } else {
                    state = SCE_CONF_DEFAULT;
                    buffer[bufferCount] = '\0';

                    // Colourize here...
                    if( strchr(buffer,'.') ) {
                        // it is an IP address...
                        styler.ColourTo(i-1,SCE_CONF_IP);
                    } else {
                        // normal number
                        styler.ColourTo(i-1,SCE_CONF_NUMBER);
                    }

                    // push back a character
                    chNext = styler[i--];
                }
                break;

        }
    }
    delete []buffer;
}



LexerModule lmLinkerScript(SCLEX_LINKERSCRIPT, ColouriseLinkerScriptDoc, "linkerScript", 0, linkerScriptWordListDesc);
