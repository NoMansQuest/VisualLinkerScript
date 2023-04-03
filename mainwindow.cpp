#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscrollbar.h"
#include "Components/QScintilla/ComponentHelpers.h"
#include "Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "ParsingEngine/CPreliminaryParser.h"
#include <QFile>
#include <memory.h>
#include <QString>
#include "ParsingEngine/CMasterParser.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::Models;

void ConstructUi(MainWindow& mainWindow);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    this->BuildUserInterface();
}


void MainWindow::BuildUserInterface()
{
    // Build Scintilla Editor
    this->m_scintilla = new QsciScintilla;
    Components::QScintilla::SetComponentStyles(*this->m_scintilla);
    auto lexer = new QsciLexerLinkerScript;
    this->m_scintilla->setLexer((QsciLexer*)lexer);

    // Add widget to Layout
    this->m_leftSegmentVerticalSpliter = new QSplitter();
    this->m_centralWidget = new QWidget();
    this->m_leftSegmentCentralWidget = new QWidget();
    this->m_leftSegmentCentralHBoxLayout = new QHBoxLayout();
    this->m_rightSegmentCentralWidget = new QWidget();
    this->m_rightSegmentCentralHBoxLayout = new QHBoxLayout();
    this->m_mainViewHorizontalSplitter = new QSplitter();
    this->m_issueTreeView = new QTreeView();
    this->m_mainViewHorizontalSplitter->addWidget(this->m_leftSegmentCentralWidget);
    this->m_mainViewHorizontalSplitter->addWidget(this->m_rightSegmentCentralWidget);
    this->m_leftSegmentCentralWidget->setLayout(this->m_leftSegmentCentralHBoxLayout);
    this->m_rightSegmentCentralWidget->setLayout(this->m_rightSegmentCentralHBoxLayout);

    ui->mainGridLayout->addWidget(this->scintilla, 0, 0, Qt::AlignCenter);
    //ui->mainGriLayout->


    // OK, We need to generate the content here
    CPreliminaryParser preliminaryParser;
    QString fileName("C:\\Temp\\TestLinkerScriptFile.lds");
    QFile fileToRead(fileName);
    fileToRead.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&fileToRead);
    auto fileContent = in.readAll();
    auto preliminaryParseResult = preliminaryParser.ProcessLinkerScript(fileName.toStdString(), fileContent.toStdString());
    auto contentToAnalyze = preliminaryParseResult->Content();

    // Generate result
    QString debugOutput = "";
    for (auto entry: contentToAnalyze)
    {
        QString translatedType;

        switch (entry.EntryType())
        {
            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::Word:
                translatedType = "Word";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::Operator:
                translatedType = "Operator";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::Assignment:
                translatedType = "Assignment";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::Number:
                translatedType = "Number";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::String:
                translatedType = "String";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::Comment:
                translatedType = "Comment";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::ParenthesisOpen:
                translatedType = "ParenthesisOpen";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::ParenthesisClose:
                translatedType = "ParenthesisClose";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::BracketOpen:
                translatedType = "BracketOpen";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::BracketClose:
                translatedType = "BracketClose";
                break;

            case VisualLinkerScript::ParsingEngine::Models::Raw::RawEntryType::Unknown:
                translatedType = "Unknown";
                break;

            default:
                translatedType = "ERROR";
                break;
        }

        /*auto formatTemplate = "Type: %1\t\t, From line %2\t to %3\t, BytePos: %4\t (%5\t bytes). P-Depth: %6\t, S-Depth: %7\t Content: %8\n";
        auto formattedOutput = QString(formatTemplate)
                .arg(translatedType)
                .arg(entry.StartLineNumber())
                .arg(entry.EndLineNumber())
                .arg(entry.StartPosition())
                .arg(entry.Length())
                .arg(entry.ParenthesisDepth())
                .arg(entry.ScopeDepth())
                .arg(fileContent.mid(entry.StartPosition(), entry.Length()));

        debugOutput.append(formattedOutput);*/

        debugOutput.append(fileContent.mid(entry.StartPosition(), entry.Length()));
    }

    // Parse content
    //CMasterParser masterParser;
    //auto parsedContent = std::move(masterParser.ProcessLinkerScriptFile(preliminaryParseResult));

    // Add more information to the output text


    // Produce output.
    this->scintilla->setText(debugOutput);
}


MainWindow::~MainWindow()
{
    delete ui;
}



