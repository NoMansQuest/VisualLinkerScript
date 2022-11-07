// This module implements the QsciLexerCMake class.
//
// Copyright (c) 2022 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexerlinkerscript.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


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
    return "cmake";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerLinkerScript::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case KeywordSet3:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case String:
    case StringLeftQuote:
    case StringRightQuote:
        return QColor(0x7f,0x00,0x7f);

    case Function:
    case BlockWhile:
    case BlockForeach:
    case BlockIf:
    case BlockMacro:
        return QColor(0x00,0x00,0x7f);

    case Variable:
        return QColor(0x80,0x00,0x00);

    case Label:
    case StringVariable:
        return QColor(0xcc,0x33,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerLinkerScript::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Function:
    case BlockWhile:
    case BlockForeach:
    case BlockIf:
    case BlockMacro:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerLinkerScript::keywords(int set) const
{  
    return 0; // None returned. The underlying lexer in 'Scintilla' will take care of that
}


// Returns the user name of a style.
QString QsciLexerLinkerScript::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case String:
        return tr("String");

    case StringLeftQuote:
        return tr("Left quoted string");

    case StringRightQuote:
        return tr("Right quoted string");

    case Function:
        return tr("Function");

    case Variable:
        return tr("Variable");

    case Label:
        return tr("Label");

    case KeywordSet3:
        return tr("User defined");

    case BlockWhile:
        return tr("WHILE block");

    case BlockForeach:
        return tr("FOREACH block");

    case BlockIf:
        return tr("IF block");

    case BlockMacro:
        return tr("MACRO block");

    case StringVariable:
        return tr("Variable within a string");

    case Number:
        return tr("Number");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerLinkerScript::defaultPaper(int style) const
{
    switch (style)
    {
    case String:
    case StringLeftQuote:
    case StringRightQuote:
    case StringVariable:
        return QColor(0xee,0xee,0xee);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerLinkerScript::refreshProperties()
{
    setAtElseProp();
}


// Read properties from the settings.
bool QsciLexerLinkerScript::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerLinkerScript::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldatelse", fold_atelse);

    return rc;
}


// Return true if ELSE blocks can be folded.
bool QsciLexerLinkerScript::foldAtElse() const
{
    return fold_atelse;
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
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}
