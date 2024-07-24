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

bool HasNonWhitespaceBeforeCurlyBracket(const std::string& str, uint32_t backwardStartingIndex);
void ReplaceLineText(const QsciScintilla* scintillaEditor, int lineNumber, const QString& newText);


CRawEntry PositionDropTest(const std::shared_ptr<CRawFile>& rawFile, const uint32_t absoluteCharacterPosition);
CRawEntry PrecedingBracketOpenOnLine(const std::shared_ptr<CRawFile>& rawFile, const uint32_t line, const uint32_t absoluteCharacterPosition);
CRawEntry SupersedingBracketCloseOnLine(const std::shared_ptr<CRawFile>& rawFile, const uint32_t line, const uint32_t absoluteCharacterPosition);
uint32_t LeadingWhiteSpaces(const std::string& stringToInspect);

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
    this->m_scintilla->setAutoIndent(false);
    this->m_scintilla->setTabWidth(4);
    this->m_scintilla->setIndentationGuides(false);
    this->m_scintilla->setIndentationGuidesBackgroundColor(QColor::fromRgb(0xff505050));
    this->m_scintilla->setIndentationGuidesForegroundColor(QColor::fromRgb(0xff909090));    
    this->m_scintilla->setIndentationsUseTabs(true);
	this->m_scintilla->setBackspaceUnindents(true);

    this->setLayout(this->m_centralLayout);

    this->m_masterParser = std::make_unique<CMasterParser>();
    this->m_linkerScriptLexer = std::make_unique<CLexer>();
    this->m_drcManager = std::make_unique<CDrcManager>();    

    // Setup deferred procedure call system
    QObject::connect(&this->m_deferredProcedureCaller, &QTimer::timeout, this, &QLinkerScriptSession::DeferredContentProcessingAction);
    QObject::connect(this->m_scintilla, &QsciScintilla::textChanged, this, &QLinkerScriptSession::EditorContentUpdated);
    QObject::connect(this->m_scintilla, &QsciScintilla::SCN_CHARADDED, this, &QLinkerScriptSession::OnCharAddedToEditor);
    //QObject::connect(this->m_scintilla, &QsciScintilla::handleCharAdded, )
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
{;
    this->m_scintilla->setText(QString::fromStdString(linkerScriptContent));    
}

void QLinkerScriptSession::EditorContentUpdated()
{
    // The lexer is always run
    auto absoluteFilePath = this->m_sessionFileInfo.AbsoluteFilePath();
    auto textToLex = this->m_scintilla->text().toStdString();
    this->m_lexedLinkerScript = this->m_linkerScriptLexer->LexLinkerScript(absoluteFilePath, textToLex);
    this->InitiateDeferredProcessing();
}

void QLinkerScriptSession::DeferredContentProcessingAction() const
{
	QElapsedTimer timer;
    timer.start();
    const auto contentToAnalyze = this->m_lexedLinkerScript->Content();
    const auto parsedLinkerScriptFile = this->m_masterParser->ProcessLinkerScriptFile(this->m_lexedLinkerScript);
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

void QLinkerScriptSession::OnCharAddedToEditor(const int charAdded) const
{
    int lineNumber;
    int index;
    this->m_scintilla->getCursorPosition(&lineNumber, &index);    

    const auto absoluteCharPosition = this->m_scintilla->positionFromLineIndex(lineNumber, index);
    const auto entryContainingCharacter = PositionDropTest(this->m_lexedLinkerScript, absoluteCharPosition);

    qDebug() << "Character '" << charAdded << "' added on line " << lineNumber << " index " << index << " - Absolute Position: " << absoluteCharPosition << Qt::endl;
    qDebug() << "RawEntry encapsulating the position: '" << static_cast<int>(entryContainingCharacter.EntryType()) << Qt::endl;
    const auto indentationData = this->m_lexedLinkerScript->IndentationData().at(lineNumber);
    const auto textAtLine = this->m_scintilla->text(lineNumber);

    // All actions performed by this module are atomic and should be undone with a single call.
    this->m_scintilla->beginUndoAction(); 

    if (charAdded == '\n')
    {
        const auto encapsulatingEntry = PositionDropTest(this->m_lexedLinkerScript, absoluteCharPosition);
        if (encapsulatingEntry.EntryType() == RawEntryType::Comment)
        {
            auto newLineContent = std::string(4 * indentationData.IndentationDepth(), ' ');
            if (indentationData.CommentLinePrefixing())
            {
                newLineContent += " *";
            }
            this->m_scintilla->insertAt(QString::fromStdString(newLineContent), lineNumber, 0);
            this->m_scintilla->setCursorPosition(lineNumber, newLineContent.length());
        }
        else if (encapsulatingEntry.EntryType() == RawEntryType::NotPresent)
        {
            // NOTE: When we receive the next-line character, the text is already going to be broken between two lines.
            //       That has to be considered.
            // Note: The current line is the line post new-line character insertion.
            const auto bracketOpen = PrecedingBracketOpenOnLine(this->m_lexedLinkerScript, lineNumber - 1, absoluteCharPosition);
            const auto bracketClose = SupersedingBracketCloseOnLine(this->m_lexedLinkerScript, lineNumber, absoluteCharPosition);

            if ((bracketOpen.EntryType() != RawEntryType::NotPresent) && 
                (bracketClose.EntryType() != RawEntryType::NotPresent))
            {
                // Extract text in between
                const auto contentLength = bracketClose.StartPosition() - bracketOpen.StartPosition() - 1;
                const auto contentBetweenBracketOpenAndCaretLength = absoluteCharPosition - bracketOpen.StartPosition() - 1;
                const auto contentBetweenCaretAndBracketCloseLength = bracketClose.StartPosition() - absoluteCharPosition;
                const auto encapsulatingText = this->m_scintilla->text().mid(bracketOpen.StartPosition() + 1, contentLength);
                const auto textBetweenBracketOpenAndCaret = this->m_scintilla->text().mid(bracketOpen.StartPosition() + 1, contentBetweenBracketOpenAndCaretLength);
                const auto textBetweenCaretAndBracketClose = this->m_scintilla->text().mid(absoluteCharPosition, contentBetweenCaretAndBracketCloseLength);


                /* NOTE: +1 for the depth because the indentation-data contains wrong depth for the current line, as the
                 *       'bracket-close' is the line as well, reducing the depth by one point (we'll be moving it to the following line though).
                 */
                auto whiteSpacesToAdd = (4 * (indentationData.IndentationDepth() + 1)) - LeadingWhiteSpaces(textAtLine.toStdString());
                auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
                auto bracketClosurePadding = std::string("\n") + std::string(4 * indentationData.IndentationDepth(), ' ');						
                this->m_scintilla->insertAt(QString::fromStdString(bracketClosurePadding), lineNumber, textBetweenCaretAndBracketClose.length());
                this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
                this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());
            }
            else if (bracketOpen.EntryType() != RawEntryType::NotPresent)
            {
                auto whiteSpacesToAdd = (4 * indentationData.IndentationDepth()) - LeadingWhiteSpaces(textAtLine.toStdString());
                auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
                this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
                this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());
            }
            else if (bracketClose.EntryType() != RawEntryType::NotPresent)
            {                 
                auto whiteSpacesToAdd = (4 * indentationData.IndentationDepth()) - LeadingWhiteSpaces(textAtLine.toStdString());
	            auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
                this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
                this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());
            }
            else
            {
                auto whiteSpacesToAdd = (4 * indentationData.IndentationDepth()) - LeadingWhiteSpaces(textAtLine.toStdString());
                auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
                this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
                this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());
            }
        }        
    }

    if (charAdded == '(')
    {
        const auto contentToInsert = std::string(")");
        this->m_scintilla->insertAt(QString::fromStdString(contentToInsert), lineNumber, index);
        this->m_scintilla->setCursorPosition(lineNumber, index);
    }

    if (charAdded == '\"')
    {
        const auto contentToInsert = std::string("\"");
        this->m_scintilla->insertAt(QString::fromStdString(contentToInsert), lineNumber, index);
        this->m_scintilla->setCursorPosition(lineNumber, index);
    }

    if (charAdded == '{')
    {        
        const auto contentToInsert = std::string("}");
        this->m_scintilla->insertAt(QString::fromStdString(contentToInsert), lineNumber, index);
        this->m_scintilla->setCursorPosition(lineNumber, index);
    }

    if (charAdded == '}')
    {
        if (HasNonWhitespaceBeforeCurlyBracket(textAtLine.toStdString(), index - 1))
        {
            return; // No action is taken.
        }
        const auto newLineContent = std::string(4 * indentationData.IndentationDepth(), ' ') + "}";
        ReplaceLineText(this->m_scintilla, lineNumber,QString::fromStdString(newLineContent));
        this->m_scintilla->setCursorPosition(lineNumber, newLineContent.length());
    }
    
    //auto lineType = this->m_scintilla->line
    this->m_scintilla->endUndoAction();
}





void ReplaceLineText(const QsciScintilla* scintillaEditor, const int lineNumber, const QString& newText)
{
    // Get the start and end positions of the line
    const int start = scintillaEditor->SendScintilla(QsciScintilla::SCI_POSITIONFROMLINE, lineNumber);
    const int end = scintillaEditor->SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, lineNumber);

    // Replace the text within that range
    scintillaEditor->SendScintilla(QsciScintilla::SCI_SETTARGETSTART, start);
    scintillaEditor->SendScintilla(QsciScintilla::SCI_SETTARGETEND, end);
    scintillaEditor->SendScintilla(QsciScintilla::SCI_REPLACETARGET, newText.length(), newText.toUtf8().constData());
}

bool HasNonWhitespaceBeforeCurlyBracket(const std::string& str, uint32_t backwardStartingIndex)
{
    // Check characters before the curly-bracket
    if (str[backwardStartingIndex] != '}')
    {
        throw std::exception("Starting index should be a closing curly bracket");
    }

    if (backwardStartingIndex == 0)
    {
        return false; // We're the first character of the line anyway.
    }

    auto hoveringIndex = backwardStartingIndex - 1;
    do
    {        
        if (!std::isspace(static_cast<unsigned char>(str[hoveringIndex]))) {
            return true;
        }
        --backwardStartingIndex;
    } while (backwardStartingIndex > 0);

    // No non-whitespace characters found before the curly-bracket
    return false;
}

CRawEntry PositionDropTest(const std::shared_ptr<CRawFile>& rawFile, const uint32_t absoluteCharacterPosition)
{
    for (const auto& entry: rawFile->Content())
	{
        if ((absoluteCharacterPosition > entry.StartPosition() + 1) && absoluteCharacterPosition < entry.StartPosition() + entry.Length())
        {
            return entry;
        }
    }

    return CRawEntry(); // Not-Present entry (i.e. nothing found).
}


CRawEntry PrecedingBracketOpenOnLine(const std::shared_ptr<CRawFile>& rawFile, uint32_t line, const uint32_t absoluteCharacterPosition)
{
    CRawEntry candidate;
    for (const auto& entry : rawFile->Content())
    {
        if (entry.EntryType() == RawEntryType::BracketOpen &&
            entry.StartLineNumber() == line &&
            entry.StartPosition() < absoluteCharacterPosition)
        {
            candidate = entry;
        }

        if (entry.StartPosition() > absoluteCharacterPosition)
        {
            break;
        }
    }

    return candidate;
}

CRawEntry SupersedingBracketCloseOnLine(const std::shared_ptr<CRawFile>& rawFile, uint32_t line, const uint32_t absoluteCharacterPosition)
{    
    for (const auto& entry : rawFile->Content())
    {
        if (entry.EntryType() == RawEntryType::BracketClose &&
            entry.StartLineNumber() == line &&
            entry.StartPosition() >= absoluteCharacterPosition)
        {
            return entry;
        }
    }

    return CRawEntry();
}

uint32_t LeadingWhiteSpaces(const std::string& stringToInspect)
{
    uint32_t count = 0;
    for (int hover = 0; hover < stringToInspect.length(); hover++)
    {
	    if (stringToInspect[hover] == ' ')
	    {
            count++;
	    }
        else if (stringToInspect[hover] == '\t')
        {
            count += 4;
        }
        else
        {
            break;
        }
    }
    return count;
}
