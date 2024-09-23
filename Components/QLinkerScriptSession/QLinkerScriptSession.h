#ifndef QLINKERSCRIPTSESSION_H__
#define QLINKERSCRIPTSESSION_H__

#include <QtWidgets>
#include <QString>
#include <memory>
#include "CLinkerScriptSessionFileInfo.h"
#include "DrcEngine/CDrcManager.h"
#include "ParsingEngine/CLexer.h"
#include "ParsingEngine/CMasterParser.h"

namespace VisualLinkerScript::ParsingEngine
{
	class CAutoStyler;
}

class QChromeTabWidget;
enum class Indicators;
enum class SearchReplaceRequestType;
enum class SearchPerimeterType;
class QsciScintilla;
class QMemoryVisualizer;
class CLinkerScriptSessionFileInfo;
class QSearchPopup;

/// @brief Object representing the full context of a loaded/created linker script file.
class QLinkerScriptSession : public QWidget
{
    Q_OBJECT

private:
    uint32_t m_sessionId;
    uint32_t m_sessionsTabIndex;
    std::shared_ptr<VisualLinkerScript::Models::CLinkerScriptFile> m_linkerScriptFile;

public:
    /// @brief Default constructor
    QLinkerScriptSession(const uint32_t sessionId, CLinkerScriptSessionFileInfo sessionFileInfo, QWidget *parent = nullptr)
        : QWidget(parent),
          m_sessionId(sessionId),
          m_sessionsTabIndex(0),		  
          m_sessionFileInfo(std::move(sessionFileInfo))
    {
        this->BuildUserInterface();
    }

    /// @brief Default destructor
    ~QLinkerScriptSession() override = default;

private:
    QMemoryVisualizer* m_memoryVisualizer;
    QsciScintilla* m_scintilla;
    QTreeView* m_issuesTreeView;
    QChromeTabWidget* m_panelsTab;
    QVBoxLayout* m_centralLayout;
    QSplitter* m_horizontalSplitter;
    QSplitter* m_verticalSplitter;  
    CLinkerScriptSessionFileInfo m_sessionFileInfo;
    QTimer m_deferredProcedureCaller;
    QSearchPopup* m_searchPopup;

    std::shared_ptr<VisualLinkerScript::ParsingEngine::CAutoStyler> m_autoStyler;
    std::unique_ptr<VisualLinkerScript::DrcEngine::CDrcManager> m_drcManager;

    std::shared_ptr<QStandardItemModel> m_violationsItemModel;
    std::shared_ptr<QStandardItem> m_lexerViolationsItem;
    std::shared_ptr<QStandardItem> m_parserViolationsItem;
    std::shared_ptr<QStandardItem> m_drcViolationsItem;

    void InitiateDeferredProcessing();
    void DeferredContentProcessingAction() const;
    void EditorContentUpdated();
    void UpdateDrcViolationsInModel() const;
    void UpdateParserViolationsInModel() const;
    void UpdateLexerViolationsInModel() const;
    void SetupViolationsView();

    // Find related
    uint32_t m_searchSessionActive = false;
    uint32_t m_searchSessionCurrentFocusedEntryStartPosition = 0;
    void ResetFindIndicators(Indicators indicatorToReset) const;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void BuildUserInterface();

public:
    /// @brief Sets the 'tabIndex' this session belongs to at higher level.
    void SetSessionsTabIndex(const uint32_t tabIndex) { this->m_sessionsTabIndex = tabIndex; }

    /// @brief Reports back the session's tab index.
    [[nodiscard]] uint32_t TabIndex() const { return this->m_sessionsTabIndex; }

    /// @brief Updates session's file information.
    void SetSessionFileInfo(const CLinkerScriptSessionFileInfo& newSessionFileInfo);

    /// @brief Reports back the session's file info.
    [[nodiscard]] CLinkerScriptSessionFileInfo SessionFileInfo(void) const { return this->m_sessionFileInfo; }

    /// @brief Reports back the linker script content.
    [[nodiscard]] std::string LinkerScriptContent(void) const;

    /// @brief: Update's linker script content. This action is reversible via "Undo".
    void SetLinkerScriptContent(const std::string& linkerScriptContent) const;

    /// TODO: DISCARD IF POSSIBLE
    /// @brief Returns back the memory visualizer widget
    QMemoryVisualizer* MemoryVisualizerWidget() const { return this->m_memoryVisualizer; }

    /// TODO: DISCARD IF POSSIBLE
    /// @brief Returns back the Scintilla Editor. 
    QsciScintilla* ScintillaEditor() const { return this->m_scintilla; }

    /// TODO: DISCARD IF POSSIBLE
    /// @brief Returns back the issues tree-view. 
    QTreeView* IssuesTreeView() const { return this->m_issuesTreeView; }

    /// @breif Make the search dialog appear.
    void ShowSearchPopup() const;

    /// @brief Positions the search popup.
    void PositionSearchPopup() const;

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

    void OnSearchReplaceRequested(
        const QString& searchText,
        const QString& replaceWith,
        bool caseMatch,
        bool wordMatch,
        bool regEx,
        SearchReplaceRequestType searchReplaceType,
        SearchPerimeterType searchPerimeter);

    void OnFindRequest(std::string searchFor, bool isRegExt, bool isCaseSensitive);
    void OnFindNext(void);
    void OnFindReplace(std::string replaceWith);
    void OnCharAddedToEditor(int charAdded) const;
    void OnEditorResize(void) const;
};

#endif // end of QLINKERSCRIPTSESSION_H__
