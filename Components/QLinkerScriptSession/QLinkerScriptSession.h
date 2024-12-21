#ifndef QLINKERSCRIPTSESSION_H__
#define QLINKERSCRIPTSESSION_H__

#include <QtWidgets>
#include <QString>
#include <string>
#include <memory>

#include "AutoStyling/CAutoStyler.h"
#include "Components/MemoryVisualizer/QMemoryVisualizer.h"
#include "Components/QScintilla/ComponentHelpers.h"
#include "CustomComponents/QIssuesTreeView.h"
#include "DrcEngine/CDrcManager.h"
#include "EditorAction/SEditorActionBase.h"
#include "ParsingEngine/CLinkerScriptParser.h"

class QChromeTabWidget;
enum class Indicators;
enum class SearchReplaceRequestType;
enum class SearchPerimeterType;
class QsciScintilla;
class QMemoryVisualizer;
class CLinkerScriptSessionFileInfo;
class QSearchPopup;

using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

/// @brief Object representing the full context of a loaded/created linker script file.
class QLinkerScriptSession : public QWidget
{

    Q_OBJECT

public:
    /// @brief End-of-line styles
    enum class EEndOfLineStyle
    {
        LF,
        CRLF
    };

    Q_ENUM(EEndOfLineStyle)
    Q_PROPERTY(EEndOfLineStyle EndOfLineStyle READ EndOfLineStyle WRITE SetEndOfLineStyle NOTIFY evEndOfLineStyleChanged)
	Q_PROPERTY(uint32_t EditorLineNumber READ EditorLineNumber NOTIFY evEditorCursorChanged)
	Q_PROPERTY(uint32_t EditorColumnIndex READ EditorColumnIndex NOTIFY evEditorCursorChanged)
    Q_PROPERTY(uint32_t EditorAbsolutePosition READ EditorAbsolutePosition NOTIFY evEditorCursorChanged)

private:
    uint32_t m_sessionId;
    uint32_t m_sessionsTabIndex;
    EEndOfLineStyle m_endOfLineStyle;
    std::shared_ptr<VisualLinkerScript::Models::CLinkerScriptFile> m_linkerScriptFile;

public:
    /// @brief Default constructor
    QLinkerScriptSession(
				const uint32_t sessionId, 
				const std::shared_ptr<VisualLinkerScript::Models::CLinkerScriptFile>& linkerScriptFile, 
				QWidget *parent = nullptr)
        : QWidget(parent),
          m_sessionId(sessionId),
          m_sessionsTabIndex(0),		  
		  m_linkerScriptFile(linkerScriptFile)
    {        
        this->BuildUserInterface();
    }

    /// @brief Default destructor
    ~QLinkerScriptSession() override = default;

private:
    QMemoryVisualizer* m_memoryVisualizer;
    QsciScintilla* m_scintilla;
    QIssuesTreeView* m_issuesTreeView;
    QChromeTabWidget* m_panelsTab;
    QVBoxLayout* m_centralLayout;
    QSplitter* m_horizontalSplitter;
    QSplitter* m_verticalSplitter;    
    QTimer m_deferredProcedureCaller;
    QSearchPopup* m_searchPopup;

    VisualLinkerScript::Components::LinkerScriptSession::AutoStyling::CAutoStyler m_autoStyler;
    VisualLinkerScript::DrcEngine::CDrcManager m_drcManager;

    std::shared_ptr<QStandardItemModel> m_violationsItemModel;
    std::shared_ptr<QStandardItem> m_lexerViolationsItem;
    std::shared_ptr<QStandardItem> m_parserViolationsItem;
    std::shared_ptr<QStandardItem> m_drcViolationsItem;       

    // Violations
    void UpdateDrcViolationsInModel() const;
    void UpdateParserViolationsInModel() const;
    void UpdateLexerViolationsInModel() const;
    void SetupViolationsView();

    static void AddViolation(
        QStandardItem& rootItem,
        const std::string& code, 
        VisualLinkerScript::Models::ESeverityCode severity, 
        const std::string& description, 
        uint32_t lineNumber, 
        uint32_t columnIndex,
        const std::string& offendingCode);

    // Editor Actions
    void ApplyEditorActions(VisualLinkerScript::SharedPtrVector<VisualLinkerScript::Components::LinkerScriptSession::EditorAction::SEditorActionBase> actions) const;
    void InitiateDeferredProcessing();
    void DeferredContentProcessingAction();
    void EditorContentUpdated();
    void SetupEditor();

    // Find related
    uint32_t m_searchSessionActive = false;
    uint32_t m_searchSessionCurrentFocusedEntryStartPosition = 0;
    void ResetFindIndicators(Indicators indicatorToReset) const;

    // Model related operations
    std::shared_ptr<CFloorPlan> GenerateFloorplan() const;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void BuildUserInterface();

public:
    /// @brief Sets the 'tabIndex' this session belongs to at higher level.
    void SetSessionsTabIndex(const uint32_t tabIndex) { this->m_sessionsTabIndex = tabIndex; }

    /// @brief Reports back the session's tab index.
    [[nodiscard]] uint32_t TabIndex() const { return this->m_sessionsTabIndex; }

    /// @brief Reports back the linker script content.
    [[nodiscard]] std::shared_ptr<VisualLinkerScript::Models::CLinkerScriptFile> LinkerScriptFile() const;

    /// @brief Reports back the end-of-line style
    [[nodiscard]] EEndOfLineStyle EndOfLineStyle() const { return this->m_endOfLineStyle; }

    /// @brief Reports back the line number the caret is on
    [[nodiscard]] uint32_t EditorLineNumber() const;

    /// @brief Reports back the total number of lines currently present in the editor
    [[nodiscard]] uint32_t EditorTotalLines() const;

    /// @brief Reports back the column index the caret is on
    [[nodiscard]] uint32_t EditorColumnIndex() const;

    /// @brief Reports back the absolute position the caret is on
    [[nodiscard]] uint32_t EditorAbsolutePosition() const;

    /// @brief Sets the end-of-line style
    void SetEndOfLineStyle(EEndOfLineStyle newEndOfLineStyle)
    {
        if (this->m_endOfLineStyle != newEndOfLineStyle)
        {
			this->m_endOfLineStyle = newEndOfLineStyle;
            emit this->evEndOfLineStyleChanged(this->m_sessionId);
        }
    }

    /// @brief Retrieves the session ID of this session.
    uint32_t SessionId() const { return this->m_sessionId; }

    /// @brief Requests the editor to jump to a specific line
    void JumpToLine(uint32_t lineNumber) const;

    /// @brief Updates content of the linker-script file. Consequently, it updates the editor and triggers an analysis.
    void UpdateContent(const std::string& newContent) const;

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
    void evEndOfLineStyleChanged(uint32_t sessionId);
    void evEditorCursorChanged(uint32_t sessionId);

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

    void OnEditorCursorPositionChanged(int line, int column);
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
