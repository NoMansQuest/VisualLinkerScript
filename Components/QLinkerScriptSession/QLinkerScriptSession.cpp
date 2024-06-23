#include "QLinkerScriptSession.h"
#include "../MemoryVisualizer/QMemoryVisualizer.h"
#include "../QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "../QScintilla/src/Qsci/qsciscintilla.h"
#include "../QScintilla/ComponentHelpers.h"
#include "../../ParsingEngine/CLexer.h"
#include "../../ParsingEngine/CMasterParser.h"
#include "../../DrcEngine/CDrcManager.h"

using namespace VisualLinkerScript::Components;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

void QLinkerScriptSession::BuildUserInterface()
{
    // Setup UI.
    this->m_issuesTreeView = new QTreeView(this);
    this->m_memoryVisualizer = new QMemoryVisualizer(this);
    this->m_scintilla = new QsciScintilla(this);
    this->m_horizontalSplitter = new QSplitter(Qt::Horizontal, this);    
    this->m_verticalSplitter = new QSplitter(Qt::Vertical, this);
    this->m_centralLayout = new QVBoxLayout(this);    

    this->m_horizontalSplitter->addWidget(this->m_scintilla);
    this->m_horizontalSplitter->addWidget(this->m_memoryVisualizer);
    this->m_horizontalSplitter->setStretchFactor(0, 2);
    this->m_horizontalSplitter->setStretchFactor(1, 1);
    this->m_verticalSplitter->addWidget(this->m_horizontalSplitter);
    this->m_verticalSplitter->addWidget(this->m_issuesTreeView);
    this->m_verticalSplitter->setStretchFactor(0, 5);
    this->m_verticalSplitter->setStretchFactor(1, 1);
    this->m_centralLayout->addWidget(this->m_verticalSplitter);
	this->m_centralLayout->setSpacing(0);
	this->m_centralLayout->setContentsMargins(0, 0, 0, 0);

    // Setup Scintilla Text Editor
	this->m_scintilla->setLexer(new QsciLexerLinkerScript);
    QScintilla::SetComponentStyles(*this->m_scintilla);
    this->setLayout(this->m_centralLayout);

    this->m_masterParser = std::make_unique<CMasterParser>();
    this->m_linkerScriptLexer = std::make_unique<CLexer>();
    this->m_drcManager = std::make_unique<CDrcManager>();

    // Setup deferred procedure call system
    QObject::connect(&this->m_deferredProcedureCaller, &QTimer::timeout, this, &QLinkerScriptSession::DeferredContentProcessingAction);
    QObject::connect(this->m_scintilla, &QsciScintilla::textChanged, this, &QLinkerScriptSession::EditorContentUpdated);
    this->m_deferredProcedureCaller.setSingleShot(true);
}

std::string QLinkerScriptSession::LinkerScriptContent() const
{
    return this->m_scintilla->text().toStdString();
}

void QLinkerScriptSession::OnFindRequest(std::string searchFor, bool isRegExt, bool isCaseSensitive)
{
    // To be implemented.
}

void QLinkerScriptSession::OnFindNext()
{
    // To be implemented.
}

void QLinkerScriptSession::OnFindReplace(std::string replaceWith)
{
    // To be implemented.
}

void QLinkerScriptSession::SetSessionFileInfo(const CLinkerScriptSessionFileInfo& newSessionFileInfo)
{
    this->m_sessionFileInfo = newSessionFileInfo;
}

void QLinkerScriptSession::SetLinkerScriptContent(const std::string& linkerScriptContent) const
{
    this->m_scintilla->setText(QString::fromStdString(linkerScriptContent));    
}

void QLinkerScriptSession::EditorContentUpdated()
{
    this->InitiateDeferredProcessing();
}

void QLinkerScriptSession::DeferredContentProcessingAction() const
{
	QElapsedTimer timer;
    timer.start();
    const auto preliminaryParseResult = this->m_linkerScriptLexer->ProcessLinkerScript(
        this->m_sessionFileInfo.AbsoluteFilePath(), 
        this->m_scintilla->text().toStdString());

    const auto contentToAnalyze = preliminaryParseResult->Content();
    const auto parsedLinkerScriptFile = this->m_masterParser->ProcessLinkerScriptFile(preliminaryParseResult);
    const auto parsingTime = timer.nsecsElapsed();
    const auto lexingAndParsingTime = (timer.nsecsElapsed() - parsingTime);    
    qDebug() << "Time consumed for parsing: " << lexingAndParsingTime << " nanoseconds" << Qt::endl;


    //this->m_drcManager->
    const auto designRuleCheckTime = (timer.nsecsElapsed() - lexingAndParsingTime);
}

void QLinkerScriptSession::InitiateDeferredProcessing()
{
    this->m_deferredProcedureCaller.start(500);
}
