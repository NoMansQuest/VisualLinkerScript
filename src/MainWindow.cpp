#include <QFile>
#include <memory>
#include <QString>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qscrollbar.h>
#include <QElapsedTimer.h>
#include "Components/InputDialogs/QJumpToLineDialog.h"
#include "Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "Components/QChromeTab/QChromeTabWidget.h"
#include "Components/QLinkerScriptSession/QLinkerScriptSession.h"
#include "LinkerScriptManager/QLinkerScriptManager.h"
#include "ParsingEngine/CLinkerScriptParser.h"
#include "MD5/MD5.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::Models;

void ConstructUi(MainWindow& mainWindow);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    this->BuildUserInterface();
    this->BuildDialogs();
    this->InitializeLinkerScriptManager();   

    // Subscribe to events
    connect(this->ui->actionExit, &QAction::triggered, this, &MainWindow::MenuActionExit);
    connect(this->ui->actionFindReplace, &QAction::triggered, this, &MainWindow::MenuActionFindReplace);
    connect(this->ui->actionNew_Linker_Script, &QAction::triggered, this, &MainWindow::MenuActionNewFile);
    connect(this->ui->actionOpen, &QAction::triggered, this, &MainWindow::MenuActionOpenFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BuildUserInterface()
{
    this->BuildUserInterfaceForStatusBar();

    this->m_contentTabRegion = new QChromeTabWidget;        
    this->setContentsMargins(0, 0, 0, 0);
    this->setCentralWidget(this->m_contentTabRegion); 
    QObject::connect(this->m_contentTabRegion, &QChromeTabWidget::evTabsCollectionUpdated, this, &MainWindow::ContentRegionTabCollectionUpdated);
    QObject::connect(this->m_contentTabRegion, &QChromeTabWidget::evActiveTabChanged, this, &MainWindow::OnActiveTabChanged);
}

void MainWindow::BuildDialogs()
{
    this->m_jumpToLineDialog = new QJumpToLineDialog(this);
    QObject::connect(this->m_jumpToLineDialog, &QJumpToLineDialog::evNavigateToLine, this, &MainWindow::OnJumpToLineRequest);
}

void MainWindow::InitializeLinkerScriptManager()
{
    this->m_linkerScriptManager = std::make_unique<QLinkerScriptManager>(this);   

    // Create Untitled session
    auto linkerScriptSession = this->m_linkerScriptManager->CreateSessionForUntitled();
    auto tabId = this->m_contentTabRegion->AddTab(linkerScriptSession, false);    
    auto formattedFileName = StringFormat("*%s", linkerScriptSession->LinkerScriptFile()->FileName().c_str());
    this->m_contentTabRegion->SetTabTitle(tabId, QString::fromStdString(formattedFileName));
    this->SubscribeToLinkerScriptSessionEvents(linkerScriptSession);
}


void MainWindow::OnEditorCursorUpdated(uint32_t linkerScriptSessionId)
{   
    if (this->ActiveLinkerScriptSession() != nullptr && this->ActiveLinkerScriptSession()->SessionId() == linkerScriptSessionId)
    {
	    auto correspondingSession = this->m_linkerScriptManager->GetSession(linkerScriptSessionId);
	    auto columnIndex = correspondingSession->EditorColumnIndex();
	    auto lineNumber = correspondingSession->EditorLineNumber();
        this->UpdateLineNumberAndColumnIndex(lineNumber, columnIndex);
    }
}

std::shared_ptr<QLinkerScriptSession> MainWindow::ActiveLinkerScriptSession() const
{
    return std::dynamic_pointer_cast<QLinkerScriptSession>(this->m_contentTabRegion->CurrentTab());    
}

void MainWindow::ContentRegionTabCollectionUpdated() const
{
    if (this->m_contentTabRegion->TabCount() == 1)
    {
        this->m_contentTabRegion->SetTabClosurePolicy(this->m_contentTabRegion->Tabs().at(0), false);
        return;
    }

    for (auto tabId : this->m_contentTabRegion->Tabs())
    {
        this->m_contentTabRegion->SetTabClosurePolicy(tabId, true);
    }
}

void MainWindow::OnActiveTabChanged(std::optional<uint32_t> activeTab)
{    
    auto currentSession = std::dynamic_pointer_cast<QLinkerScriptSession>(this->m_contentTabRegion->CurrentTab());
    if (currentSession != nullptr)
    {        
        auto columnIndex = currentSession->EditorColumnIndex();
        auto lineNumber = currentSession->EditorLineNumber();
        this->UpdateLineNumberAndColumnIndex(lineNumber, columnIndex);
    }
    else
    {
        this->UpdateLineNumberAndColumnIndex(0,0);
    }    
}

void MainWindow::SaveLastUsedDirectory(const QString& directory) {
    QSettings settings("Nasser.G64", "VisualLinkerScript");
    settings.setValue("lastUsedDirectory", directory);
}

QString MainWindow::GetLastUsedDirectory() {
    QSettings settings("Nasser.G64", "VisualLinkerScript");
    return settings.value("lastUsedDirectory", QDir::homePath()).toString();
}

void MainWindow::BuildUserInterfaceForStatusBar()
{
	// Build status-bar buttons
	this->m_statusBarMessageLabel = new QLabel("Ready!", this->statusBar());
	this->m_statusBarLineColumnButton = new QPushButton("Ln 214, Col 38", this->statusBar());
	this->m_statusBarSpacesButton = new QPushButton("Spaces: 4", this->statusBar());
	this->m_statusBarEncodingButton = new QPushButton("UTF-8", this->statusBar());
	this->m_statusBarLineEndingButton = new QPushButton("CRLF", this->statusBar());
	this->m_statusBarPositionButton = new QPushButton("Pos: 544", this->statusBar());

	this->statusBar()->addPermanentWidget(this->m_statusBarMessageLabel, 1);
	this->statusBar()->addPermanentWidget(this->m_statusBarLineColumnButton, 0);
	this->statusBar()->addPermanentWidget(this->m_statusBarSpacesButton, 0);
	this->statusBar()->addPermanentWidget(this->m_statusBarEncodingButton, 0);
	this->statusBar()->addPermanentWidget(this->m_statusBarLineEndingButton, 0);
	this->statusBar()->addPermanentWidget(this->m_statusBarPositionButton, 0);

    QObject::connect(this->m_statusBarLineColumnButton, &QPushButton::clicked, this, &MainWindow::EditorActionShowJumpToLine);
}

void MainWindow::OnJumpToLineRequest(const uint32_t newLine) const
{
    auto currentSession = std::dynamic_pointer_cast<QLinkerScriptSession>(this->m_contentTabRegion->CurrentTab());
    if (currentSession != nullptr)
    {
        currentSession->JumpToLine(newLine);
    }
}

void MainWindow::MenuActionExit(bool checked)
{
    emit this->evExitRequested();
}

void MainWindow::MenuActionNewFile()
{        
    auto linkerScriptSession = this->m_linkerScriptManager->CreateSessionForUntitled();
    auto tabId = this->m_contentTabRegion->AddTab(linkerScriptSession, false);
    auto formattedFileName = StringFormat("*%s", linkerScriptSession->LinkerScriptFile()->FileName().c_str());
    this->m_contentTabRegion->SetTabTitle(tabId, QString::fromStdString(formattedFileName));
    this->m_contentTabRegion->NavigateToTab(tabId);
    this->SubscribeToLinkerScriptSessionEvents(linkerScriptSession);
}

void MainWindow::MenuActionOpenFile()
{
    auto fileName = QFileDialog::getOpenFileName(this, 
        "Open LinkerScript File", 
        this->GetLastUsedDirectory(),
        "LinkerScript Files (*.lds);;All Files (*)");

    if (fileName.isEmpty()) {
        return;
    }
            
    std::string fileContent;
    std::string errorMessage;
    if (!ReadFileContent(fileName.toStdString(), fileContent, errorMessage))
    {
        QMessageBox::warning(nullptr, "Error", QString::fromStdString(errorMessage));
        return;
    }

    QFileInfo fileInfo(fileName);

    // We save this for next use.
    SaveLastUsedDirectory(fileInfo.absolutePath());

    char signatureBuffer[16];
    char signatureString[64];
    MD5::CMD5Hasher md5Hash(fileContent.c_str(), fileInfo.size(), signatureBuffer);
    MD5::sig_to_string(signatureBuffer, signatureString, 64);

    auto linkerScriptFile = std::make_shared<CLinkerScriptFile>(
        fileName.toStdString(),
        fileInfo.absoluteFilePath().toStdString(),
        fileContent,
        true,
        signatureString);

    auto linkerScriptSession = this->m_linkerScriptManager->CreateSessionForExistingFile(linkerScriptFile);
    linkerScriptSession->UpdateContent(fileContent);
    auto tabId = this->m_contentTabRegion->AddTab(linkerScriptSession, false);        
    this->m_contentTabRegion->SetTabTitle(tabId, QString::fromStdString(linkerScriptFile->FileName()));
    this->m_contentTabRegion->NavigateToTab(tabId);
    this->SubscribeToLinkerScriptSessionEvents(linkerScriptSession);
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
    if (!this->m_contentTabRegion->CurrentTabId().has_value())
    {
        return; // We should not have been activated anyway;
    }

    auto currentTabId = this->m_contentTabRegion->CurrentTabId().value();
    auto session = std::dynamic_pointer_cast<QLinkerScriptSession>(this->m_contentTabRegion->GetTabContent(currentTabId));
    session->ShowSearchPopup();
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

void MainWindow::StatusBarLineColumnButtonPressed()
{

}

void MainWindow::StatusBarSpacesButtonPressed()
{

}

void MainWindow::StatusBarEncodingButtonPressed()
{

}

void MainWindow::StatusBarLineEndingButtonPressed()
{

}

void MainWindow::StatusBarPositionButtonPressed()
{

}

void MainWindow::EditorActionShowJumpToLine() const
{
	const auto currentSession = std::dynamic_pointer_cast<QLinkerScriptSession>(this->m_contentTabRegion->CurrentTab());
    this->m_jumpToLineDialog->setRange(currentSession->EditorTotalLines(), currentSession->EditorLineNumber());
    this->m_jumpToLineDialog->move(this->m_statusBarLineColumnButton->mapToGlobal(QPoint(0, -1)));
    this->m_jumpToLineDialog->show();
    this->m_jumpToLineDialog->activateWindow();
    this->m_jumpToLineDialog->setFocusToTextEdit();
}

void MainWindow::EditorActionShowEolSelector()
{
	
}

void MainWindow::EditorActionShowEncodingSelector()
{
	
}

void MainWindow::SubscribeToLinkerScriptSessionEvents(std::shared_ptr<QLinkerScriptSession> linkerScriptSession)
{
    QObject::connect(linkerScriptSession.get(), &QLinkerScriptSession::evEditorCursorChanged, this, &MainWindow::OnEditorCursorUpdated);
}

void MainWindow::UnsubscribeFromLinkerScriptSessionEvents(std::shared_ptr<QLinkerScriptSession> linkerScriptSession)
{
    QObject::disconnect(linkerScriptSession.get(), &QLinkerScriptSession::evEditorCursorChanged, this, &MainWindow::OnEditorCursorUpdated);
}

void MainWindow::UpdateLineNumberAndColumnIndex(uint32_t lineNumber, uint32_t columnIndex)
{
    this->m_statusBarLineColumnButton->setText("Ln " + QString::number(lineNumber) + ", Col " + QString::number(columnIndex));
}

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
CLinkerScriptLexer linkerScriptLexer;
QString fileName("C:\\Temp\\GccDefaultLinkerScriptFile.lds");
QFile fileToRead(fileName);
fileToRead.open(QFile::ReadOnly | QFile::Text);
QTextStream in(&fileToRead);
auto fileContent = in.readAll();
auto preliminaryParseResult = linkerScriptLexer.LexLinkerScript(fileName.toStdString(), fileContent.toStdString());
auto contentToAnalyze = preliminaryParseResult->ParsedContent();

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
auto parsedLinkerScriptFile = masterParser.ParseLinkerScriptFile(preliminaryParseResult);
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


// Some other code
    //auto newDialog = new QDialog();
//   auto centralWidget = new QWidget(this);
//   centralWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//   auto widgetToAdd = new QPushButton(centralWidget);
//   widgetToAdd->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
//   widgetToAdd->setFixedHeight(100);
//   auto newVBoxLayout = new QVBoxLayout(centralWidget);
   //newVBoxLayout->addWidget(widgetToAdd);
   //this->setCentralWidget(centralWidget);


   //auto centralWidget = new QWidget(this);
//   auto searchPopup = new QSearchPopup(false, centralWidget);

//   searchPopup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//   searchPopup->OnTextNotFound();
//   auto newVBoxLayout = new QVBoxLayout(centralWidget);
//   newVBoxLayout->addWidget(searchPopup);
//   newVBoxLayout->addWidget(new QTextEdit(centralWidget));
//   this->setCentralWidget(centralWidget);



//////////////////// New File Click
    auto centralWidget = new QWidget(this);
    auto chromeTab = new QChromeTabWidget(centralWidget);
    auto newVBoxLayout = new QVBoxLayout(centralWidget);
    newVBoxLayout->addWidget(chromeTab);
    this->setCentralWidget(centralWidget);
    chromeTab->SetTabTitle(chromeTab->AddTab(std::shared_ptr<QWidget>(new QTextEdit), true), "Funny Tabddddddddddddddddddddddd");
    chromeTab->SetTabTitle(chromeTab->AddTab(std::shared_ptr<QWidget>(new QPushButton)), "Funny Tab2");
    chromeTab->SetTabTitle(chromeTab->AddTab(std::shared_ptr<QWidget>(new QLabel("Funny Label"))), "Funny Tab3");

    // To be implemented
    this->m_contentTabRegion->SetTabTitle(this->m_contentTabRegion->AddTab(std::shared_ptr<QWidget>(new QLinkerScriptSession(1)), false), "DummyLinkerScript.lds");


   */


