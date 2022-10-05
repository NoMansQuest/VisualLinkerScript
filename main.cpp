#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Untitled.ld - Visual Linker Script");

    // Set Stylesheet for the main window
    QFile file(":/resources/Stylesheets/default.qss");
    file.open(QFile::ReadOnly);
    auto styleSheet = QString(file.readAll());
    mainWindow.setStyleSheet(styleSheet);

    // Show & Run
    mainWindow.show();
    return application.exec();
}
