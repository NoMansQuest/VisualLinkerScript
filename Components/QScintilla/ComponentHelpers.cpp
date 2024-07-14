#include "ComponentHelpers.h"
#include <QScrollBar>
#include <QStyle>
#include "src/Qsci/qsciscintilla.h"
#include <QOperatingSystemVersion>

#include "Scintilla.h"

using namespace VisualLinkerScript::Components;

void QScintilla::SetComponentStyles(QsciScintilla& target)
{
    target.setColor(QColor::fromRgba(0xffffffff));
    target.setCaretForegroundColor(QColor::fromRgba(0xfff0f0f0));
    target.setCaretLineVisible(true);
	target.setCaretLineFrameWidth(2);
    target.setCaretLineBackgroundColor(0xff464646);

    target.setMargins(3);
    target.setMarginsBackgroundColor(QColor::fromRgba(0xff161616)); 
    target.setMarginsForegroundColor(QColor::fromRgba(0xff707070));
    target.setMarginWidth(0, 50);
    target.setTabWidth(4);

    target.setMarginType(0, QsciScintilla::NumberMargin);
    target.setMarginWidth(0, 50);
        
    target.setMarginType(1, QsciScintilla::SymbolMargin);    
    target.setMarginBackgroundColor(1, QColor::fromRgba(0xff161616));
    target.setFolding(QsciScintilla::PlainFoldStyle, 1);
    target.setFoldMarginColors(QColor::fromRgb(0xff262626), QColor::fromRgba(0xff161616));
    target.setMarkerForegroundColor(QColor::fromRgb(0xff464646), SC_MARK_ARROWDOWN);
    target.setMarkerBackgroundColor(QColor::fromRgb(0xff464646), SC_MARK_ARROWDOWN);    
    target.setMarginWidth(1, 16);

    target.setMarginType(2, QsciScintilla::SymbolMarginDefaultBackgroundColor);
    target.setMarginWidth(2, 5);

    // Font-Related
#ifdef Q_OS_LINUX
  target.setFont(QFont("Monospace", 9));
#endif
#ifdef Q_OS_WIN32
  target.setFont(QFont("Consolas", 11));
#endif

    // Set Lexer
    //target.setLexer()
}

void QScintilla::SetTextEditorStyles(QsciScintilla& target)
{
    UNREFERENCED_PARAMETER(target); // TODO: Fix this later on.
}

