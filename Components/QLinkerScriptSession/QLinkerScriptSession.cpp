#include "QLinkerScriptSession.h"
#include "../MemoryVisualizer/QMemoryVisualizer.h"
#include "../QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "../QScintilla/src/Qsci/qsciscintilla.h"
#include "../QScintilla/ComponentHelpers.h"

using namespace VisualLinkerScript::Components;

void QLinkerScriptSession::BuildUserInterface()
{
    this->m_issuesTreeView = new QTreeView(this);
    this->m_memoryVisualizer = new QMemoryVisualizer(this);
    this->m_scintilla = new QsciScintilla(this);
    this->m_horizontalSplitter = new QSplitter(Qt::Horizontal, this);    
    this->m_verticalSplitter = new QSplitter(Qt::Vertical, this);
    this->m_centralLayout = new QVBoxLayout(this);    

    this->m_horizontalSplitter->addWidget(this->m_scintilla);
    this->m_horizontalSplitter->addWidget(this->m_memoryVisualizer);
    this->m_verticalSplitter->addWidget(this->m_horizontalSplitter);
    this->m_verticalSplitter->addWidget(this->m_issuesTreeView);
    this->m_centralLayout->addWidget(this->m_verticalSplitter);
	this->m_centralLayout->setSpacing(0);
	this->m_centralLayout->setContentsMargins(0, 0, 0, 0);

	auto lexer = new QsciLexerLinkerScript;
	this->m_scintilla->setLexer((QsciLexer*)lexer);
    Components::QScintilla::SetComponentStyles(*this->m_scintilla);


    this->setLayout(this->m_centralLayout);
}

void QLinkerScriptSession::OnFindRequest(QString searchFor, bool isRegExt, bool isCaseSensitive)
{
    // To be implemented.
}

void QLinkerScriptSession::OnFindNext()
{
    // To be implemented.
}

void QLinkerScriptSession::OnFindReplace(QString replaceWith)
{
    // To be implemented.
}
