#ifndef COMPONENTHELPERS_H
#define COMPONENTHELPERS_H

class QsciScintilla;

namespace Components::QScintilla
{
    /**
        * @brief As QSciScintilla cannot be stylized via QStyleSheet, we need to set those here.
        * @param target The target QsciScintilla component.
        */
    void SetComponentStyles(QsciScintilla& target);

    /**
        * @brief Text-Editor styles (including coloring) will be set here.
        * @param target The target QsciScintilla component.
        */
    void SetTextEditorStyles(QsciScintilla& target);    
}


#endif // COMPONENTHELPERS_H
