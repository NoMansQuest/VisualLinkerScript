#ifndef QLINKERSCRIPTSESSION_H__
#define QLINKERSCRIPTSESSION_H__

#include <QtWidgets>
#include <QString>
#include <vector>
#include <memory>

class QsciScintilla;

namespace VisualLinkerScript::Components
{
    class QMemoryVisualizer;

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
        QMemoryVisualizer* m_memoryVisualizer;
        QsciScintilla* m_scintilla;
        QTreeView* m_issuesTreeView;
        QVBoxLayout* m_centralLayout;
        QSplitter* m_horizontalSplitter;
        QSplitter* m_verticalSplitter;

    protected:
        void BuildUserInterface();

    public:
        QMemoryVisualizer* MemoryVisualizerWdiget() { return this->m_memoryVisualizer; }
        QsciScintilla* ScintillaEditor() { return this->m_scintilla; }
        QTreeView* IssuesTreeView() { return this->m_issuesTreeView; }
    };
};


#endif
