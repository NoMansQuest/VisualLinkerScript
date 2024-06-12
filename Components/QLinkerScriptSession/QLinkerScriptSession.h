#ifndef QLINKERSCRIPTSESSION_H__
#define QLINKERSCRIPTSESSION_H__

#include <QtWidgets>
#include <QString>
#include <vector>
#include <memory>
#include "CLinkerScriptSessiosnFileInfo.h"

class QsciScintilla;
class QMemoryVisualizer;
class CLinkerScriptSessionFileInfo;

/// @brief The main tab component to emulate google chrome's tab component.
class QLinkerScriptSession : public QWidget
{
    Q_OBJECT

private:
    uint32_t m_sessionId;
    uint32_t m_tabIndex;

public:
    /// @brief Default constructor
    QLinkerScriptSession(uint32_t sessionId, CLinkerScriptSessionFileInfo sessionFileInfo, QWidget *parent = 0)
        : QWidget(parent),
          m_tabIndex(0),
          m_sessionId(sessionId),
          m_sessionFileInfo(sessionFileInfo)
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
    CLinkerScriptSessionFileInfo m_sessionFileInfo;

protected:
    void BuildUserInterface();

public:
    void SetTabIndex(uint32_t tabIndex) { this->m_tabIndex = tabIndex; }    
    uint32_t TabIndex() { return this->m_tabIndex; }
    void SetSessionFileInfo(CLinkerScriptSessionFileInfo newSessionFileInfo);        
    CLinkerScriptSessionFileInfo SessionFileInfo(void) const { return this->m_sessionFileInfo; }
    std::string LinkerScriptContent(void);
    QMemoryVisualizer* MemoryVisualizerWdiget() { return this->m_memoryVisualizer; }
    QsciScintilla* ScintillaEditor() { return this->m_scintilla; }
    QTreeView* IssuesTreeView() { return this->m_issuesTreeView; }

signals:
    void evIssueSelected(uint32_t sessionId, uint32_t issueId);
    void evLinkerScriptContentChanged(uint32_t sessionId);
    void evUndo(uint32_t sessionId);
    void evRedo(uint32_t sessionId);
    void evCut(uint32_t sessionId);
    void evCopy(uint32_t sessionId);
    void evPaste(uint32_t sessionId);

    void evSessionFileInfoChanged(void);
    void evTextPositionChanged(uint32_t sessionId, int line, int index);
    void evCopyAvailable(uint32_t sessionId, bool yes);
    void evIndicatorClicked(uint32_t sessionId, int line, int index, Qt::KeyboardModifiers state);
    void evIndicatorReleased(uint32_t sessionId, int line, int index, Qt::KeyboardModifiers state);
    void evLinesChanged(uint32_t sessionId);
    void evMarginClicked(uint32_t sessionId, int margin, int line, Qt::KeyboardModifiers state);
    void evMarginRightClicked(uint32_t sessionId, int margin, int line, Qt::KeyboardModifiers state);
    void evModificationAttempted(uint32_t sessionId);
    void evModificationChanged(uint32_t sessionId, bool m);
    void evSelectionChanged(uint32_t sessionId);
    void evUserListActivated(uint32_t sessionId, int id, std::string activatorString);

public slots:
    void OnFindRequest(std::string searchFor, bool isRegExt, bool isCaseSensitive);
    void OnFindNext();
    void OnFindReplace(std::string replaceWith);
};


#endif // end of QLINKERSCRIPTSESSION_H__
