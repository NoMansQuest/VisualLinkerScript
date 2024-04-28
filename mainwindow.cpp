#include <QFile>
#include <memory>
#include <QString>
#include <QTabWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscrollbar.h"
#include "QElapsedTimer.h"
#include "Components/QScintilla/ComponentHelpers.h"
#include "Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "Components/QSearchPopup/QSearchPopup.h"
#include "ParsingEngine/CLexer.h"
#include "ParsingEngine/CMasterParser.h"
#include "Messaging/CEventAggregator.h"
#include "Messaging/UserInitiated/COpenFileRequest.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Components;


void ConstructUi(MainWindow& mainWindow);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    this->BuildUserInterface();

    // Subscribe to events
    connect(this->ui->actionExit, &QAction::triggered, this, &MainWindow::MenuActionExit);
    connect(this->ui->actionFindReplace, &QAction::triggered, this, &MainWindow::MenuActionFindReplace);
    connect(this->ui->actionNew_Linker_Script, &QAction::triggered, this, &MainWindow::MenuActionNewFile);
}


void MainWindow::BuildUserInterface()
{
    // Build Scintilla Editor
    /*
    this->m_scintilla = new QsciScintilla;    
    auto lexer = new QsciLexerLinkerScript;
    this->m_scintilla->setLexer((QsciLexer*)lexer);
    this->ui->centralwidget->setObjectName("baseM1");
    Components::QScintilla::SetComponentStyles(*this->m_scintilla);

    // Add widget to Layout
    this->m_memoryVisualizer = new QWidget;
    this->m_memoryVisualizer->setObjectName("baseM0");
    this->m_memoryVisualizer->setContentsMargins(0,0,0,0);

    this->m_centralWidgetLayout = new QHBoxLayout;    
    this->m_centralWidgetLayout->setContentsMargins(0,0,0,0);

    this->m_centralWidgetVSplitter = new QSplitter;
    this->m_centralWidgetVSplitter->setContentsMargins(0,0,0,0);

    this->m_topHalfWidget = new QWidget;
    this->m_topHalfWidget->setObjectName("baseM0");
    this->m_topHalfWidget->setContentsMargins(0,0,0,0);

    this->m_topHalfWidgetHSplitter = new QSplitter;
    this->m_topHalfWidgetHSplitter->setContentsMargins(0,0,0,0);

    this->m_topHalfWidgetLayout = new QHBoxLayout;
    this->m_topHalfWidgetLayout->setObjectName("baseM0");
    this->m_topHalfWidgetLayout->setContentsMargins(0,0,0,0);

    this->m_bottomHalfWidgetLayout = new QHBoxLayout;    
    this->m_bottomHalfWidgetLayout->setContentsMargins(0,0,0,0);

    this->m_bottomtHalfWidget = new QWidget;
    this->m_bottomtHalfWidget->setObjectName("baseM0");
    this->m_bottomtHalfWidget->setContentsMargins(0,0,0,0);

    this->m_issuesTreeView = new QTreeView;
    this->m_issuesTreeView->setContentsMargins(0,0,0,0);

    this->m_contentTabRegion = new QTabWidget;
    this->m_contentTabRegion->setTabsClosable(true);
    this->m_contentTabRegion->addTab(this->m_scintilla, "MainLinkerScrip.ld");
    this->m_contentTabRegion->addTab(new QWidget, "MainLinkerScrip.ld2");
    this->m_contentTabRegion->addTab(new QWidget, "MainLinkerScrip.ld3");
    this->m_contentTabRegion->widget(0)->setContentsMargins(0, 10, 0, 0);

    this->m_topHalfWidgetHSplitter->setOrientation(Qt::Orientation::Horizontal);
    this->m_topHalfWidgetHSplitter->addWidget(this->m_contentTabRegion);
    this->m_topHalfWidgetHSplitter->addWidget(this->m_memoryVisualizer);
    //this->m_topHalfWidgetHSplitter->addWidget(this->_)
    this->m_topHalfWidgetLayout->addWidget(this->m_topHalfWidgetHSplitter);
    this->m_topHalfWidget->setLayout(this->m_topHalfWidgetLayout);
    this->m_centralWidgetVSplitter->setOrientation(Qt::Orientation::Vertical);
    this->m_centralWidgetVSplitter->addWidget(this->m_topHalfWidget);
    this->m_bottomHalfWidgetLayout->addWidget(this->m_issuesTreeView);
    this->m_bottomtHalfWidget->setLayout(this->m_bottomHalfWidgetLayout);
    this->m_centralWidgetVSplitter->addWidget(this->m_bottomtHalfWidget);
    this->m_centralWidgetLayout->addWidget(this->m_centralWidgetVSplitter);
    this->ui->centralwidget->setLayout(this->m_centralWidgetLayout);

    // OK, We need to generate the content here
    CLexer linkerScriptLexer;
    QString fileName("C:\\Temp\\GccDefaultLinkerScriptFile.lds");
    QFile fileToRead(fileName);
    fileToRead.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&fileToRead);
    auto fileContent = in.readAll();
    auto preliminaryParseResult = linkerScriptLexer.ProcessLinkerScript(fileName.toStdString(), fileContent.toStdString());
    auto contentToAnalyze = preliminaryParseResult->Content();

    // Generate result
    QString debugOutput = "";
    for (auto entry: contentToAnalyze)
    {
        QString translatedType;

        switch (entry.EntryType())
        {
            case VisualLinkerScript::Models::Raw::RawEntryType::Word:
                translatedType = "Word";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::ArithmeticOperator:
                translatedType = "ArithmeticOperator";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::Semicolon:
                translatedType = "Semicolon";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::Comma:
                translatedType = "Comma";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::AssignmentOperator:
                translatedType = "Assignment";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::Number:
                translatedType = "Number";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::String:
                translatedType = "String";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::Comment:
                translatedType = "Comment";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::ParenthesisOpen:
                translatedType = "ParenthesisOpen";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::ParenthesisClose:
                translatedType = "ParenthesisClose";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::BracketOpen:
                translatedType = "BracketOpen";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::BracketClose:
                translatedType = "BracketClose";
                break;

            case VisualLinkerScript::Models::Raw::RawEntryType::Unknown:
                translatedType = "Unknown";
                break;

            default:
                translatedType = "ERROR";
                break;
        }

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

        debugOutput.append(fileContent.mid(entry.StartPosition(), entry.Length()));
    }

    // Parse content
    CMasterParser masterParser;

    QElapsedTimer timer;
    timer.start();
    auto parsedLinkerScriptFile = masterParser.ProcessLinkerScriptFile(preliminaryParseResult);
    auto parsingTime = timer.nsecsElapsed();
    auto parsedContentDebugInfo = parsedLinkerScriptFile->ToDebugInfo(0);
    auto toDebugInfoTime = timer.nsecsElapsed() - parsingTime;
    auto targetString = QString::fromStdString(parsedContentDebugInfo);

    targetString.append("\n ParsingTime (ns): ");
    targetString.append(QString::fromStdString(std::to_string(parsingTime)));
    targetString.append("\n ToDebugInfo time (ns): ");
    targetString.append(QString::fromStdString(std::to_string(toDebugInfoTime)));
    this->m_scintilla->setText(targetString);

    // Add more information to the output text


    // Produce output.    
    // this->m_scintilla->setText(fileContent);
    */
}

void MainWindow::MenuActionExit(bool checked)
{
    emit this->EventExitRequested();
}

void MainWindow::MenuActionNewFile()
{
    auto newDialog = new QDialog();
    auto newVBoxLayout = new QVBoxLayout(newDialog);
    auto searchPopup = new QSearchPopup(false);        
    newVBoxLayout->addWidget(searchPopup);       
    newVBoxLayout->setAlignment(Qt::AlignTop);
    searchPopup->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);    
    newDialog->setLayout(newVBoxLayout);
    newDialog->resize(300, 100);
    newDialog->show();        
    
    // To be implemented
}

void MainWindow::MenuActionOpenFile()
{
    // To be implemented
}

void MainWindow::MenuActionSaveFile()
{
    // To be implemented
}

void MainWindow::MenuActionSaveFileAs()
{
    // To be implemented
}

void MainWindow::MenuActionCloseFile()
{
    // To be implemented
}

void MainWindow::MenuActionReload()
{
    // To be implemented
}

void MainWindow::MenuActionCut()
{
    // To be implemented
}

void MainWindow::MenuActionCopy()
{
    // To be implemented
}

void MainWindow::MenuActionPaste()
{
    // To be implemented
}

void MainWindow::MenuActionUndo()
{
    // To be implemented
}

void MainWindow::MenuActionRedo()
{
    // To be implemented
}

void MainWindow::MenuActionFindReplace()
{
    //SearchDialog* searchDialog = new SearchDialog();
    //searchDialog->show();
}

void MainWindow::MenuActionSelectAll()
{
    // To be implemented
}

void MainWindow::MenuActionLoadMappingFile()
{
    // To be implemented
}

void MainWindow::MenuActionHelp()
{
    // To be implemented
}


MainWindow::~MainWindow()
{
    delete ui;
}



