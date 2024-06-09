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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QChromeTabWidget;
QT_END_NAMESPACE

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

private slots:
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

signals:
    void EventNewFileRequested(void);
    void EventNewFileFromTemplateRequested(QString templateName);

    void EventOpenFileRequested(QString fileName);
    void EventSaveFileRequested(QString fileName);
    void EventCloseFileRequeasted(void);
    void EventReloadFileRequested(void);
    void EventExitRequested(void);

    void EventCutRequested(void);
    void EventCopyRequested(void);
    void EventPasteRequested(void);
    void EventSelectAllRequested(void);
    void EventFileReplaceRequested(void);
    void EventLoadMappingFileRequested(void);
    void EventHelpRequested(void);

private:    
    void BuildUserInterface(void);
    void BuildUserInterfaceForStatusBar(void);
};
#endif // MAINWINDOW_H
