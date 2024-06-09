#ifndef QLINKERSCRIPTSESSION_H__
#define QLINKERSCRIPTSESSION_H__

#include <QtWidgets>
#include <QString>
#include <vector>
#include <memory>

class QsciScintilla;
class QMemoryVisualizer;

/// @brief The main tab component to emulate google chrome's tab component.
class QLinkerScriptSession : public QWidget
{
    Q_OBJECT

private:
    uint32_t m_sessionId;

public:
    /// @brief Default constructor
    QLinkerScriptSession(uint32_t sessionId, QWidget *parent = 0)
        : QWidget(parent),
            m_sessionId(sessionId)
    {
        this->BuildUserInterface();
    }

    /// @brief Default destructor
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

signals:
    void SignalIssueSelected(uint32_t sessionId, uint32_t issueId);
    void SignalLinkerScriptContentChanged(uint32_t sessionId);
    void SignalUndo(uint32_t sessionId);
    void SignalRedo(uint32_t sessionId);
    void SignalCut(uint32_t sessionId);
    void SignalCopy(uint32_t sessionId);
    void SignalPaste(uint32_t sessionId);

    void SignalTextPositionChanged(uint32_t sessionId, int line, int index);
    void SignalCopyAvailable(uint32_t sessionId, bool yes);
    void SignalIndicatorClicked(uint32_t sessionId, int line, int index, Qt::KeyboardModifiers state);
    void SignalIndicatorReleased(uint32_t sessionId, int line, int index, Qt::KeyboardModifiers state);
    void SignalLinesChanged(uint32_t sessionId);
    void SignalMarginClicked(uint32_t sessionId, int margin, int line, Qt::KeyboardModifiers state);
    void SignalMarginRightClicked(uint32_t sessionId, int margin, int line, Qt::KeyboardModifiers state);
    void SignalModificationAttempted(uint32_t sessionId);
    void SignalModificationChanged(uint32_t sessionId, bool m);
    void SignalSelectionChanged(uint32_t sessionId);
    void SignalUserListActivated(uint32_t sessionId, int id, QString string);

public slots:
    void OnFindRequest(QString searchFor, bool isRegExt, bool isCaseSensitive);
    void OnFindNext();
    void OnFindReplace(QString replaceWith);

};


#endif // end of QLINKERSCRIPTSESSION_H__
