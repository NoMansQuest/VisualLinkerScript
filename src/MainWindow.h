#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QGridLayout>
#include <QSplitter>
#include <QLabel>
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <memory>

class QLinkerScriptSession;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QChromeTabWidget;
class QLinkerScriptManager;
class QJumpToLineDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
	QChromeTabWidget* m_contentTabRegion;
    QVBoxLayout* m_centralLayout;    
	Ui::MainWindow* ui;
    QLabel* m_statusBarMessageLabel;
    QPushButton* m_statusBarLineColumnButton;
    QPushButton* m_statusBarSpacesButton;
    QPushButton* m_statusBarEncodingButton;
    QPushButton* m_statusBarLineEndingButton;
    QPushButton* m_statusBarPositionButton;
    std::unique_ptr<QLinkerScriptManager> m_linkerScriptManager;

    QJumpToLineDialog* m_jumpToLineDialog;

private:
    void SaveLastUsedDirectory(const QString& directory);
    QString GetLastUsedDirectory(void);
    void SubscribeToLinkerScriptSessionEvents(std::shared_ptr<QLinkerScriptSession> linkerScriptSession);
    void UnsubscribeFromLinkerScriptSessionEvents(std::shared_ptr<QLinkerScriptSession> linkerScriptSession);
    void UpdateLineNumberAndColumnIndex(uint32_t lineNumber, uint32_t columnIndex);

private slots:
    void ContentRegionTabCollectionUpdated(void) const;

    void MenuActionExit(bool checked);
    void MenuActionNewFile(void);
    void MenuActionOpenFile(void);
    void MenuActionSaveFile(void);
    void MenuActionSaveFileAs(void);
    void MenuActionCloseFile(void);
    void MenuActionReload(void);
    void MenuActionCut(void);
    void MenuActionCopy(void);
    void MenuActionPaste(void);
    void MenuActionUndo(void);
    void MenuActionRedo(void);
    void MenuActionFindReplace(void);
    void MenuActionSelectAll(void);
    void MenuActionLoadMappingFile(void);
    void MenuActionHelp(void);

    void StatusBarLineColumnButtonPressed(void);
    void StatusBarSpacesButtonPressed(void);
    void StatusBarEncodingButtonPressed(void);
    void StatusBarLineEndingButtonPressed(void);
    void StatusBarPositionButtonPressed(void);

    void OnEditorCursorUpdated(uint32_t linkerScriptSessionId);
    void OnActiveTabChanged(std::optional<uint32_t> activeTab);
    void OnJumpToLineRequest(uint32_t newLine) const;

    void EditorActionShowJumpToLine() const;
    void EditorActionShowEolSelector();
    void EditorActionShowEncodingSelector();

signals:
    void evNewFileRequested(void);
    void evNewFileFromTemplateRequested(QString templateName);

    void evOpenFileRequested(QString fileName);
    void evSaveFileRequested(QString fileName);
    void evCloseFileRequeasted(void);
    void evReloadFileRequested(void);
    void evExitRequested(void);

    void evCutRequested(void);
    void evCopyRequested(void);
    void evPasteRequested(void);
    void evSelectAllRequested(void);
    void evFileReplaceRequested(void);
    void evLoadMappingFileRequested(void);
    void evHelpRequested(void);

private:    
    void BuildUserInterface(void);
    void BuildUserInterfaceForStatusBar(void);
    void BuildDialogs(void);
    void InitializeLinkerScriptManager(void);
    std::shared_ptr<QLinkerScriptSession> ActiveLinkerScriptSession() const;
};
#endif // MAINWINDOW_H
