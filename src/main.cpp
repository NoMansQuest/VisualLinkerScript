#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QRegularExpression>

struct PaletteEntryStruct
{
    QString entryName;
    QString entryValue;
};


QString GetStylesheetContent();
std::vector<PaletteEntryStruct> ExtractPaletteEntries(const QString& paletteFileContent);

QString GetStylesheetContent()
{
    // Set Stylesheet for the main window
    QFile colorPalettefile(":/resources/Stylesheets/colorpalette.txt");
    QFile styleSheetfile(":/resources/Stylesheets/default.qss");
    styleSheetfile.open(QFile::ReadOnly);
    colorPalettefile.open(QFile::ReadOnly);
    auto styleSheetFileContent = QString(styleSheetfile.readAll());
    auto colorPaletteFileContent = QString(colorPalettefile.readAll());
    auto paletteEntries = ExtractPaletteEntries(colorPaletteFileContent);

    for (auto paletteEntry: paletteEntries)
    {
        styleSheetFileContent.replace(paletteEntry.entryName, paletteEntry.entryValue);
    }

    styleSheetfile.close();
    colorPalettefile.close();
    return styleSheetFileContent;
}

std::vector<PaletteEntryStruct> ExtractPaletteEntries(const QString& paletteFileContent)
{
    std::vector<PaletteEntryStruct> result;
    auto splitContent = paletteFileContent.split(QRegularExpression ("[\r\n]"), Qt::SkipEmptyParts);
    for (auto lineEntry: splitContent)
    {
        auto splitLine = lineEntry.split("=");
        auto name = splitLine[0].trimmed();
        auto value = splitLine[1].trimmed();
        result.emplace_back(PaletteEntryStruct{name, value});
    }
    return result;
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Untitled.ld - Visual Linker Script");

    // Apply the styles
    application.setStyleSheet(GetStylesheetContent());

    // Show & Run
    mainWindow.show();
    return application.exec();
}
