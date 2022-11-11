
#include "ComponentHelpers.h"
#include <QScrollBar>
#include <QStyle>
#include "src/Qsci/qsciscintilla.h"
#include <QOperatingSystemVersion>

void Components::QScintilla::SetComponentStyles(QsciScintilla& target)
{
    // Color styling
    target.setWhitespaceBackgroundColor(QColor::fromRgb(0,0,0));
    target.setPaper(QColor::fromRgba(0xff121212));
    target.setCaretForegroundColor(QColor::fromRgba(0xFFF0F0F0));
    target.setColor(QColor::fromRgba(0xFFFFFFFF));

    target.setMargins(2);
    target.setMarginsBackgroundColor(QColor::fromRgba(0xff121212));
    target.setMarginsForegroundColor(QColor::fromRgba(0xff959595));

    target.setMarginType(0, QsciScintilla::NumberMargin);
    target.setMarginWidth(0, 50);

    target.setMarginType(1, QsciScintilla::SymbolMargin);
    target.setMarginWidth(1, 16);

    // Font-Related
#ifdef Q_OS_LINUX
  target.setFont(QFont("Monospace", 10));
#endif
#ifdef Q_OS_WIN32
  target.setFont(QFont("Consolas", 10));
#endif

    // Set Lexer
    //target.setLexer()
}

void Components::QScintilla::SetTextEditorStyles(QsciScintilla& target)
{
    UNREFERENCED_PARAMETER(target); // TODO: Fix this later on.
}
