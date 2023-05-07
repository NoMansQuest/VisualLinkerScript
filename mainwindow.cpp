#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscrollbar.h"
#include "Components/QScintilla/ComponentHelpers.h"
#include "Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "ParsingEngine/CPreliminaryParser.h"
#include <QFile>
#include <memory.h>
#include <QString>
#include <QTabWidget>
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
    this->m_scintilla->setText(fileContent);
}


void MainWindow::BuildBottomHalfOfScreen(QWidget* centralWidget)
{

}

void MainWindow::BuildTopHalfOfScreen(QWidget* centralWidget)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}



