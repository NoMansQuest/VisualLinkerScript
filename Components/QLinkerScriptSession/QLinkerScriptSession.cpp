#include "QLinkerScriptSession.h"
#include "../MemoryVisualizer/QMemoryVisualizer.h"
#include "../QScintilla/src/Qsci/qsciscintilla.h"

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

    this->setLayout(this->m_centralLayout);
}
