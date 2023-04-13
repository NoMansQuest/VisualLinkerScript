#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QGridLayout>
#include <QSplitter>
#include <QWidget>
#include <QHBoxLayout>
#include "Components/QScintilla/src/qsci/qsciscintilla.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

private:    
    void BuildUserInterface();
    void BuildBottomHalfOfScreen(QWidget* centralWidget);
    void BuildTopHalfOfScreen(QWidget* centralWidget);

private:
    QHBoxLayout* m_centralWidgetLayout;
    QSplitter* m_centralWidgetVSplitter;
    QWidget* m_topHalfWidget;
    QHBoxLayout* m_topHalfWidgetLayout;
    QSplitter* m_topHalfWidgetHSplitter;
    QWidget* m_bottomtHalfWidget;
    QHBoxLayout* m_bottomHalfWidgetLayout;

    QWidget* m_memoryVisualizer;
    QsciScintilla* m_scintilla;
    QTreeView* m_issuesTreeView;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
