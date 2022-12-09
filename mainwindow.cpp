#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscrollbar.h"
#include "Components/QScintilla/ComponentHelpers.h"
#include "Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "ParsingEngine/CPreliminaryParser.h"
#include <QFile>
#include <memory.h>
#include <QString>

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::Models;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    this->scintilla = new QsciScintilla;
    Components::QScintilla::SetComponentStyles(*this->scintilla);    
    auto lexer = new QsciLexerLinkerScript;
    this->scintilla->setLexer((QsciLexer*)lexer);
    ui->splitter->addWidget(this->scintilla);    

    // OK, We need to generate the content here
    CPreliminaryParser preliminaryParser;
    QFile fileToRead("C:\\Temp\\TestLinkerScriptFile.lds");
    fileToRead.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&fileToRead);
    auto fileContent = in.readAll();
    auto parseResult = preliminaryParser.ProcessLinkerScript(fileContent.toStdString());

    // Generate result
    QString debugOutput = "";
    for (auto entry: *parseResult->Content())
    {
        QString translatedType;

        switch (entry.EntryType())
        {
            case VisualLinkerScript::ParsingEngine::Models::EntryType::Word:
                translatedType = "Word";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::Operator:
                translatedType = "Operator";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::Assignment:
                translatedType = "Assignment";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::Number:
                translatedType = "Number";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::String:
                translatedType = "String";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::Comment:
                translatedType = "Comment";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::ParenthesisOpen:
                translatedType = "ParenthesisOpen";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::ParenthesisClose:
                translatedType = "ParenthesisClose";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::BracketOpen:
                translatedType = "BracketOpen";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::BracketClose:
                translatedType = "BracketClose";
                break;

            case VisualLinkerScript::ParsingEngine::Models::EntryType::Unknown:
                translatedType = "Unknown";
                break;

            default:
                translatedType = "ERROR";
                break;
        }

       /*
        auto formatTemplate = "Type: %1\t\t, From line %2\t to %3\t, BytePos: %4\t (%5\t bytes). P-Depth: %6\t, S-Depth: %7\t Content: %8\n";
        auto formattedOutput = QString(formatTemplate)
                .arg(translatedType)
                .arg(entry.StartLineNumber())
                .arg(entry.EndLineNumber())
                .arg(entry.StartPosition())
                .arg(entry.Length())
                .arg(entry.ParenthesisDepth())
                .arg(entry.ScopeDepth())
                .arg(fileContent.mid(entry.StartPosition(), entry.Length()));

        debugOutput.append(formattedOutput);
        */
        debugOutput.append(fileContent.mid(entry.StartPosition(), entry.Length()));
    }

    this->scintilla->setText(debugOutput);


}

MainWindow::~MainWindow()
{
    delete ui;
}
