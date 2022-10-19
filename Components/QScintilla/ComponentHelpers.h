#ifndef COMPONENTHELPERS_H
#define COMPONENTHELPERS_H

class QsciScintilla;

namespace Components
{
    namespace QScintilla
    {
        /**
         * @brief As QSciScintilla cannot be stylized via QStyleSheet, we need to set those here.
         * @param target The target component to set styles for.
         */
        void SetComponentStyles(QsciScintilla& target);
    }
}


#endif // COMPONENTHELPERS_H
