// This defines the interface to the QsciLexerLinkerScript class.
//
// Author: Nasser Ghoseiri. 
// (This is an external contribution)


#ifndef QSCILEXERLINKERSCRIPT_H
#define QSCILEXERLINKERSCRIPT_H

#include <QObject>

#include "Components/QScintilla/src/Qsci/qsciglobal.h"
#include "Components/QScintilla/src/Qsci/qscilexer.h"


//! \brief The QsciLexerLinkerScript class encapsulates the Scintilla LinkerScript lexer.
class QSCINTILLA_EXPORT QsciLexerLinkerScript : public QsciLexer
{
    Q_OBJECT

public:


    //! This enum defines the meanings of the different styles used by the
    //! LinkerScript lexer.
    enum {
        //! The default.
        Default = 0,

        //! Reserved keyword in linker script (such as 'Sections', 'Phdrs', etc.)
        Reserved = 1,

        //! Represents a LinkerScript function
        Function = 2,

        //! Curly Brackets (i.e. '{' and '}'
        Curly_Brackets = 3,

        //! Parenthesis
        Parenthesis = 4,

        //! Operators (such as '+', '=', '&', etc.)
        Operators = 5,

        //! Location Counter... the 'dot'
        Location_Counter = 6,

        //! Comments
        Comments = 7,

        //! Double-quotation, indicating the start of a string
        String_Start = 8,

        //! String content, basically everything inbetween double-quotations
        String_Content = 9,

        //! Double-quotation, indicating the end of a string
        String_End = 10,

        //! Decimal number
        Decimal_Numebr = 11,

        //! Octal number
        Octal_Number = 12,

        //! Hexadecimal number
        Hexadecimal_Number = 13,

        //! Binary number
        Binary_Number = 14,

        //! Uknown word, neither function nor reserved. This could be a symbol name.
        Unknown_Word = 15,

        //! Malformatted number. This should be shown as an error.
        Unknown_Number = 16      
    };

    //! Construct a QsciLexerLinkerScript with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerLinkerScript(QObject *parent = 0);

    //! Destroys the QsciLexerLinkerScript instance.
    virtual ~QsciLexerLinkerScript();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the propertyChanged()
    //! signal as required.
    void refreshProperties();

    //! Returns true if ELSE blocks can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const;

public slots:
    //! If \a fold is true then ELSE blocks can be folded.  The default is
    //! false.
    //!
    //! \sa foldAtElse()
    virtual void setFoldAtElse(bool fold);

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setAtElseProp();

    bool fold_atelse;

    QsciLexerLinkerScript(const QsciLexerLinkerScript &);
    QsciLexerLinkerScript &operator=(const QsciLexerLinkerScript &);
};

#endif
