// This module implements the QsciLexerLinkerScript class.
//
// Copyright (c) 2022 Nasser GHOSEIRI
// 
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexerlinkerscript.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

namespace
{
    QColor colorCodeWhite(200,200,200);
    QColor colorVividGreen(78,201,176);
    QColor colorStaleGreen(87,176,74);
    QColor colorTooVividGreen(181,206,168);
    QColor colorBeige(214,157,133);
    QColor colorViolet(189,99,197);
    QColor colorDarkGray(127,127,127);
    QColor colorLightGray(155,155,155);
    QColor colorWhite(200,200,200);
    QColor colorVividBlue(185,156,214);
    QColor colorYellow(255,215,4);
    QColor colorVividRed(237,28,36);
    QColor colorDarkPaper(18,18,18);
}



// The ctor.
QsciLexerLinkerScript::QsciLexerLinkerScript(QObject *parent)
    : QsciLexer(parent), fold_atelse(false)
{
}


// The dtor.
QsciLexerLinkerScript::~QsciLexerLinkerScript()
{
}


// Returns the language name.
const char *QsciLexerLinkerScript::language() const
{
    return "LinkerScript";
}


// Returns the lexer name.
const char *QsciLexerLinkerScript::lexer() const
{
    return "linkerScript";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerLinkerScript::defaultColor(int style) const
{    
    switch (style)
    {   
        case Location_Counter:
        case Reserved:
            return colorVividBlue;

        case Function:
            return colorVividGreen;

        case Curly_Brackets:
            return colorViolet;

        case Parenthesis:
            return colorYellow;        
                    
        case Comments:
            return colorStaleGreen;

        case String_Start:            
        case String_Content:
        case String_End:
            return colorBeige;

        case Decimal_Numebr:
        case Octal_Number:
        case Hexadecimal_Number:
        case Binary_Number:
            return colorTooVividGreen;
    
        case Unknown_Number:
            return colorVividRed;

        case Unknown_Word:
        case Operators:
        case Default:
        default:
            return colorWhite;
    }    
}


// Returns the font of the text for a style.
QFont QsciLexerLinkerScript::defaultFont(int style) const
{
    #if defined(Q_OS_WIN)
            return QFont("Consolas", 11);
    #elif defined(Q_OS_MAC)
            return QFont("Consolas", 11);
    #else
            return QFont("Ubuntu Mono",11);
    #endif
}


// Returns the set of keywords.
const char *QsciLexerLinkerScript::keywords(int set) const
{  
    UNREFERENCED_PARAMETER(set);
    return 0; // None returned. The underlying lexer in 'Scintilla' will take care of that
}


// Returns the user name of a style.
QString QsciLexerLinkerScript::description(int style) const
{
    switch (style)
    {   
        case Location_Counter:
            return tr("Location-Counter (the dot)");

        case Reserved:
            return tr("Reserved keywords");

        case Function:
            return tr("Function names");

        case Curly_Brackets:
            return tr("Curley Brackets");

        case Parenthesis:
            return tr("Parenthesis");
                    
        case Comments:
            return tr("Comments");

        case String_Start:     
            return tr("Double-quotation begining string");

        case String_Content:
            return tr("String content");

        case String_End:
            return tr("Double-quotation ending the string");

        case Decimal_Numebr:
            return tr("Decimal number");

        case Octal_Number:
            return tr("Octal number");

        case Hexadecimal_Number:
            return tr("Hexadecimal number");

        case Binary_Number:
            return tr("Binary number");
    
        case Unknown_Number:
            return tr("Malformed number");

        case Unknown_Word:
            return tr("Unrecognizable word (potentially symbols)");

        case Operators:
            return tr("Operators");

        case Default:
            return tr("Default Text");

        default:
            return QString();
    }    
}


// Returns the background colour of the text for a style.
QColor QsciLexerLinkerScript::defaultPaper(int style) const
{
    return colorDarkPaper;
}


// Refresh all properties.
void QsciLexerLinkerScript::refreshProperties()
{
    // No actions
}


// Read properties from the settings.
bool QsciLexerLinkerScript::readProperties(QSettings &qs,const QString &prefix)
{
    UNREFERENCED_PARAMETER(qs);
    UNREFERENCED_PARAMETER(prefix);
    return false;
}


// Write properties to the settings.
bool QsciLexerLinkerScript::writeProperties(QSettings &qs,const QString &prefix) const
{
    UNREFERENCED_PARAMETER(qs);
    UNREFERENCED_PARAMETER(prefix);
    return false;
}


// Return true if ELSE blocks can be folded.
bool QsciLexerLinkerScript::foldAtElse() const
{
    return false;
}


// Set if ELSE blocks can be folded.
void QsciLexerLinkerScript::setFoldAtElse(bool fold)
{
    fold_atelse = fold;
    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerLinkerScript::setAtElseProp()
{
    // No action undertaken yet
}
