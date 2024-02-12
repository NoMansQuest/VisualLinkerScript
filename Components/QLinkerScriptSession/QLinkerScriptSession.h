#ifndef QLINKERSCRIPTSESSION_H__
#define QLINKERSCRIPTSESSION_H__

#include <QtWidgets>
#include <QString>
#include <vector>
#include <memory>
#include "../QScintilla/src/qsci/qsciscintilla.h"

namespace VisualLinkerScript::Components
{
    /// @brief The main tab component to emulate google chrome's tab compoonent.
    class QLinkerScriptSession : public QWidget
    {
        Q_OBJECT

    public:
        /// @brief Default constructor
        QLinkerScriptSession(QWidget *parent = 0)
            : QWidget(parent)
        {
        }

        /// @brief Default desctructor
        ~QLinkerScriptSession()
        {}


    private:
        QWidget* m_memoryVisualizer;
        QsciScintilla* m_scintilla;
        QTreeView* m_issuesTreeView;
    };
};


#endif
