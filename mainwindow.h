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
    /// @brief Constructs user-interface of this window
    void BuildUserInterface();

private:
    QWidget* m_centralWidget;
    QWidget* m_leftSegmentCentralWidget;
    QHBoxLayout* m_leftSegmentCentralHBoxLayout;
    QWidget* m_rightSegmentCentralWidget;
    QHBoxLayout* m_rightSegmentCentralHBoxLayout;
    QSplitter* m_mainViewHorizontalSplitter;
    QSplitter* m_leftSegmentVerticalSpliter;

    QsciScintilla* m_scintilla;
    QTreeView* m_issueTreeView;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
