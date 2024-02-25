#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QGridLayout>
#include <QSplitter>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
namespace VisualLinkerScript::Components { class QChromeTab; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void MenuActionExit(bool checked);
    void MenuActionNewFile();
    void MenuActionOpenFile();
    void MenuActionSaveFile();
    void MenuActionSaveFileAs();
    void MenuActionCloseFile();
    void MenuActionReload();
    void MenuActionCut();
    void MenuActionCopy();
    void MenuActionPaste();
    void MenuActionUndo();
    void MenuActionRedo();
    void MenuActionFindReplace();
    void MenuActionSelectAll();
    void MenuActionLoadMappingFile();
    void MenuActionHelp();

signals:
    void EventNewFileRequested();
    void EventNewFileFromTemplateRequested(QString templateName);

    void EventOpenFileRequested(QString fileName);
    void EventSaveFileRequested(QString fileName);
    void EventCloseFileRequeasted();
    void EventReloadFileRequested();
    void EventExitRequested();

    void EventCutRequested();
    void EventCopyRequested();
    void EventPasteRequested();
    void EventSelectAllRequested();
    void EventFileReplaceRequested();
    void EventLoadMappingFileRequested();
    void EventHelpRequested();

private:    
    void BuildUserInterface();

private:
    VisualLinkerScript::Components::QChromeTab* m_contentTabRegion;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
