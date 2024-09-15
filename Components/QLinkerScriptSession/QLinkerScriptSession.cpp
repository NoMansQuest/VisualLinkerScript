#include "QLinkerScriptSession.h"
#include "../MemoryVisualizer/QMemoryVisualizer.h"
#include "../QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include "../QScintilla/src/Qsci/qsciscintilla.h"
#include "../QScintilla/ComponentHelpers.h"
#include "../../ParsingEngine/CLexer.h"
#include "../../ParsingEngine/CMasterParser.h"
#include "../../DrcEngine/CDrcManager.h"
#include "Components/QChromeTab/QChromeTabWidget.h"
#include "Components/QSearchPopup/QSearchPopup.h"


/// @brief Enum contains all defined indicators.
enum class Indicators : int
{
    IndicatorFind = 0,
	IndicatorLexerViolation,
    IndicatorDrcViolation,
    IndicatorParserViolation,
};


/// @brief Represents matched result when 'find' is performed on the text
struct SearchMatchResult {
    uint32_t start;
    uint32_t length;
};

using namespace VisualLinkerScript::Components;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::DrcEngine;

bool HasNonWhitespaceBeforeCurlyBracket(const std::string& str, uint32_t backwardStartingIndex);

std::vector<SearchMatchResult> SearchForContent(
     const QString& contentToSearchIn, 
     const QString& whatToSearchFor,
     bool caseMatch,
     bool regEx,
     bool wordMatch);

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
    this->m_searchPopup = new QSearchPopup(this->m_scintilla);
    this->m_panelsTab = new QChromeTabWidget(this);    
    this->m_searchPopup->hide();

    this->m_horizontalSplitter->addWidget(this->m_scintilla);
    this->m_horizontalSplitter->addWidget(this->m_memoryVisualizer);
    this->m_horizontalSplitter->setStretchFactor(0, 2);
    this->m_horizontalSplitter->setStretchFactor(1, 1);
    this->m_verticalSplitter->addWidget(this->m_horizontalSplitter);
    this->m_verticalSplitter->addWidget(this->m_panelsTab);
    this->m_verticalSplitter->setStretchFactor(0, 5);
    this->m_verticalSplitter->setStretchFactor(1, 1);
    this->m_centralLayout->addWidget(this->m_verticalSplitter);
	this->m_centralLayout->setSpacing(0);
	this->m_centralLayout->setContentsMargins(0, 0, 0, 0);

    // Panels tab
    auto issuesTab = this->m_panelsTab->AddTab(std::shared_ptr<QWidget>(this->m_issuesTreeView), true);
    this->m_panelsTab->SetTabTitle(issuesTab, "Violations");


    this->setLayout(this->m_centralLayout);

    this->m_masterParser = std::make_unique<CMasterParser>();
    this->m_linkerScriptLexer = std::make_unique<CLexer>();
    this->m_drcManager = std::make_unique<CDrcManager>();

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
    this->m_scintilla->installEventFilter(this);

    this->m_scintilla->indicatorDefine(QsciScintilla::RoundBoxIndicator, static_cast<int>(Indicators::IndicatorFind));
    this->m_scintilla->setIndicatorOutlineColor(QColor::fromRgb(0, 0, 0, 0), static_cast<int>(Indicators::IndicatorFind));
    this->m_scintilla->setIndicatorForegroundColor(QColor::fromRgb(0xff, 0xff, 0xff, 0x1F), static_cast<int>(Indicators::IndicatorFind));
    this->m_scintilla->setSelectionBackgroundColor(QColor::fromRgb(00, 00, 0xff, 0x5F));


    // Setup deferred procedure call system
    QObject::connect(&this->m_deferredProcedureCaller, &QTimer::timeout, this, &QLinkerScriptSession::DeferredContentProcessingAction);
    this->m_deferredProcedureCaller.setSingleShot(true);

    // Connect Scintilla editor signals
    QObject::connect(this->m_scintilla, &QsciScintilla::textChanged, this, &QLinkerScriptSession::EditorContentUpdated);
    QObject::connect(this->m_scintilla, &QsciScintilla::SCN_CHARADDED, this, &QLinkerScriptSession::OnCharAddedToEditor);
    QObject::connect(this->m_scintilla, &QsciScintilla::resized, this, &QLinkerScriptSession::OnEditorResize);
    //QObject::connect(this->m_scintilla, &QsciScintilla::handleCharAdded, )

    // Connect search popup signals
    QObject::connect(this->m_searchPopup, &QSearchPopup::evSearchReplaceRequested, this, &QLinkerScriptSession::OnSearchReplaceRequested);

    // Make final calls...    
    this->PositionSearchPopup();
    this->SetupViolationsView();
}

void QLinkerScriptSession::SetupViolationsView() const
{    
    // Create the QStandardItemModel with 4 columns (1 for item and 3 for additional data)    
    QStandardItemModel* model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({ "Violation", "Code", "Description", "Line", "File"});

    // Populate the model with some hierarchical data
    QStandardItem* parentItem = model->invisibleRootItem();

    // First root item with 3 columns of data
    QStandardItem* item1 = new QStandardItem("Root Item 1");
    QStandardItem* item1_col1 = new QStandardItem("Data 1-1");
    QStandardItem* item1_col2 = new QStandardItem("Data 1-2");
    item1->setEditable(false);

    parentItem->appendRow({ item1, item1_col1, item1_col2 });

    // Adding a child to the first root item
    QStandardItem* child1 = new QStandardItem("Child Item 1.1");
    QStandardItem* child1_col1 = new QStandardItem("Data 1.1-1");
    QStandardItem* child1_col2 = new QStandardItem("Data 1.1-2");
    child1->setEditable(false);

    item1->appendRow({ child1, child1_col1, child1_col2 });

    // Second root item with 3 columns of data
    QStandardItem* item2 = new QStandardItem("Root Item 2");
    QStandardItem* item2_col1 = new QStandardItem("Data 2-1");
    QStandardItem* item2_col2 = new QStandardItem("Data 2-2");
    item2->setEditable(false);

    parentItem->appendRow({ item2, item2_col1, item2_col2 });

    // Set the model to the QTreeView
    this->m_issuesTreeView->setModel(model);

    // Enable alternating row colors
    this->m_issuesTreeView->setAlternatingRowColors(true);
}

void QLinkerScriptSession::OnEditorResize() const
{
    this->PositionSearchPopup();
}

std::string QLinkerScriptSession::LinkerScriptContent() const
{
    return this->m_scintilla->text().toStdString();
}

void QLinkerScriptSession::PositionSearchPopup() const
{    
    QSize parentSize = this->m_scintilla->size();
    this->m_searchPopup->updateGeometry();
    int popupWidth = this->m_searchPopup->width();
    int popupHeight = this->m_searchPopup->sizeHint().height();    
    int xPos = parentSize.width() - popupWidth - (this->m_scintilla->verticalScrollBar()->isVisible() ? this->m_scintilla->verticalScrollBar()->width() : 0);
    int yPos = 0;  // Align to top    
    this->m_searchPopup->setGeometry(xPos, yPos, popupWidth, popupHeight);    
}

void QLinkerScriptSession::ShowSearchPopup() const
{
    this->m_searchPopup->ShowPopup(this->m_scintilla->hasSelectedText());    
    this->PositionSearchPopup();
    this->m_searchPopup->FocusOnSearch(this->m_scintilla->selectedText().toStdString());
}

void QLinkerScriptSession::OnSearchReplaceRequested(
	const QString& searchText, 
    const QString& replaceWith,
    bool caseMatch,
	bool wordMatch,
    bool regEx, 
    SearchReplaceRequestType searchReplaceType, 
    SearchPerimeterType searchPerimeter)
{
    switch (searchReplaceType)
    {
	    case SearchReplaceRequestType::FindNext:
	    {
            /*
            auto found = this->m_scintilla->findFirst(searchText, regEx, caseMatch, wordMatch, true, true);
            if (!found)
            {
                break;
            }
            */

            this->ResetFindIndicators(Indicators::IndicatorFind);
            auto foundEntries = SearchForContent(this->m_scintilla->text(), searchText, caseMatch, regEx, wordMatch);
            auto focusedFoundEntryPositionSet = false;
            SearchMatchResult resultToFocusOn;

            for (const auto entry: foundEntries)
            {
                int lineNumber;
                int columnPosition;
                this->m_scintilla->lineIndexFromPosition(entry.start, &lineNumber, &columnPosition);

                this->m_scintilla->fillIndicatorRange(
                    lineNumber,
                    columnPosition,
                    lineNumber,
                    columnPosition + searchText.length(),
                    (int)Indicators::IndicatorFind);

                if (!focusedFoundEntryPositionSet)
                {
                    if ((!this->m_searchSessionActive) || (this->m_searchSessionCurrentFocusedEntryStartPosition == foundEntries.back().start))
                    {
                        focusedFoundEntryPositionSet = entry.start;
                        resultToFocusOn = entry;
                        this->m_searchSessionActive = true;
                        this->m_searchSessionCurrentFocusedEntryStartPosition = entry.start;
                        focusedFoundEntryPositionSet = true;
                    }
                    else
                    {
                        if (entry.start > this->m_searchSessionCurrentFocusedEntryStartPosition)
                        {
                            this->m_searchSessionCurrentFocusedEntryStartPosition = entry.start;
                            resultToFocusOn = entry;
                            focusedFoundEntryPositionSet = true;
                        }
                    }
                    
                }             
            }

            if (focusedFoundEntryPositionSet) // This means we have a result to focus on
            {
                int startLine, startColumn, endLine, endColumn;
                this->m_scintilla->lineIndexFromPosition(resultToFocusOn.start, &startLine, &startColumn);
                this->m_scintilla->lineIndexFromPosition(resultToFocusOn.start + resultToFocusOn.length, &endLine, &endColumn);
                this->m_scintilla->setSelection(startLine, startColumn, endLine, endColumn);
                this->m_scintilla->ensureLineVisible(startLine);
            }

	        break;
	    }

	    case SearchReplaceRequestType::FindPrevious:
		{
            this->m_scintilla->findFirst(searchText, regEx, caseMatch, wordMatch, true, false);
			break;
		}

	    case SearchReplaceRequestType::FindAll:
	    {
            // This is a completely different topic (needs a tree-view explorer below)
		    break;
	    }

	    case SearchReplaceRequestType::ReplaceNext:
		{

		    break;
		}
	    case SearchReplaceRequestType::ReplaceAll:
	    {
            // This is a completely different topic, needs replacing the entire text.
	        break;
	    }

	    default:
	    {
	        throw std::exception("Unsupported enum value detected.");
	    }
    }


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

bool QLinkerScriptSession::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) 
    {
        const auto keyEvent = dynamic_cast<QKeyEvent*>(event);
	    if (keyEvent->key() == Qt::Key_Escape) 
        {
            this->m_searchPopup->hide();
            this->ResetFindIndicators(Indicators::IndicatorFind);
            this->m_scintilla->setSelection(0, 0, 0, 0);
            this->m_searchSessionActive = false;
            this->m_searchSessionCurrentFocusedEntryStartPosition = false;
        }
    }    
    return QWidget::eventFilter(obj, event);
}

void QLinkerScriptSession::ResetFindIndicators(Indicators indicatorToReset) const
{
    this->m_scintilla->clearIndicatorRange(
        0, 
        this->m_scintilla->length() > 0 ? this->m_scintilla->length() - 1 : 0,
        (int)indicatorToReset);
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
        else if (encapsulatingEntry.EntryType() == RawEntryType::String)
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

            if ((bracketOpen.EntryType() != RawEntryType::NotPresent) && (bracketClose.EntryType() != RawEntryType::NotPresent))
            {
                // Extract text in between
                int bracketOpenLineNumber, bracketOpenIndex;
                this->m_scintilla->lineIndexFromPosition(bracketOpen.StartPosition(), &bracketOpenLineNumber, &bracketOpenIndex);

                const auto contentLength = bracketClose.StartPosition() - bracketOpen.StartPosition() - 1;
                const auto bracketOpenToCaretTextLength = absoluteCharPosition - bracketOpen.StartPosition() - 1;
                const auto caretToBracketCloseTextLength = bracketClose.StartPosition() - absoluteCharPosition;
                const auto encapsulatingText = this->m_scintilla->text().mid(bracketOpen.StartPosition() + 1, contentLength);
                const auto bracketOpenToCaretText = this->m_scintilla->text().mid(bracketOpen.StartPosition() + 1, bracketOpenToCaretTextLength).toStdString();
                const auto caretToBracketCloseText = this->m_scintilla->text().mid(absoluteCharPosition, caretToBracketCloseTextLength).toStdString();

                const auto bracketClosePadding = std::string(4 * indentationData.IndentationDepth(), ' ');
                const auto newLinePadding = std::string(4 * (indentationData.IndentationDepth() + 1), ' ');
                const auto bracketOpenLineText = this->m_scintilla->text(bracketOpenLineNumber).mid(0, bracketOpenIndex + 1).toStdString();
                const auto newLineText = newLinePadding + bracketOpenToCaretText + caretToBracketCloseText;
            	const auto bracketCloseText = bracketClosePadding + this->m_scintilla->text(lineNumber).toStdString();

            	this->m_scintilla->insertAt(QString("\n"), lineNumber, index);
                this->m_scintilla->setLineText(QString::fromStdString(newLineText), lineNumber);
                this->m_scintilla->setLineText(QString::fromStdString(bracketOpenLineText), lineNumber - 1);
                this->m_scintilla->setLineText(QString::fromStdString(bracketCloseText), lineNumber + 1);
                this->m_scintilla->setCursorPosition(lineNumber, newLineText.length() - 1);

            }
            else if (bracketOpen.EntryType() != RawEntryType::NotPresent)
            {
                auto leadingWhiteSpaceCount = LeadingWhiteSpaces(textAtLine.toStdString());
                auto minimumWhiteSpaceCount = (4 * (indentationData.IndentationDepth()));
                auto whiteSpacesToAdd = leadingWhiteSpaceCount > minimumWhiteSpaceCount ? 0 : minimumWhiteSpaceCount - leadingWhiteSpaceCount;
                auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
                this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
                this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());
            }
            else if (bracketClose.EntryType() != RawEntryType::NotPresent)
            {
                auto leadingWhiteSpaceCount = LeadingWhiteSpaces(textAtLine.toStdString());
                auto minimumWhiteSpaceCount = (4 * (indentationData.IndentationDepth()));
                auto whiteSpacesToAdd = leadingWhiteSpaceCount > minimumWhiteSpaceCount ? 0 : minimumWhiteSpaceCount - leadingWhiteSpaceCount;
	            auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
                this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
                this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());
            }
            else
            {                
                auto linePadCount = (4 * (indentationData.IndentationDepth()));
                auto newLinePadding = std::string(linePadCount, ' ');
                auto newText = newLinePadding + textAtLine.trimmed().toStdString();
                this->m_scintilla->setLineText(QString::fromStdString(newText), lineNumber);
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
        this->m_scintilla->setLineText(QString::fromStdString(newLineContent), lineNumber);
        this->m_scintilla->setCursorPosition(lineNumber, newLineContent.length());
    }
    
    //auto lineType = this->m_scintilla->line
    this->m_scintilla->endUndoAction();
}


std::vector<SearchMatchResult> SearchForContent(
    const QString& contentToSearchIn, 
    const QString& whatToSearchFor, 
    const bool caseMatch, 
    const bool regEx, 
    const bool wordMatch)
{
    std::vector<SearchMatchResult> results;

    // Case sensitivity setting
    Qt::CaseSensitivity caseSensitivity = caseMatch ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if (regEx) {
        // Regular expression mode
        QString pattern = whatToSearchFor;

        if (wordMatch) {
            pattern = "\\b" + QRegularExpression::escape(whatToSearchFor) + "\\b";
        }

        QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
        if (!caseMatch) {
            options |= QRegularExpression::CaseInsensitiveOption;
        }

        QRegularExpression regex(pattern, options);
        QRegularExpressionMatchIterator it = regex.globalMatch(contentToSearchIn);

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                SearchMatchResult result;
                result.start = static_cast<uint32_t>(match.capturedStart());
                result.length = static_cast<uint32_t>(match.capturedLength());
                results.emplace_back(result);
            }
        }
    }
    else {
        // Simple string search mode
        int startPos = 0;

        while ((startPos = contentToSearchIn.indexOf(whatToSearchFor, startPos, caseSensitivity)) != -1) {
            if (wordMatch) {
                // Check if the match is a whole word
                bool isWholeWord = true;

                // Check preceding character
                if (startPos > 0) {
                    QChar precedingChar = contentToSearchIn[startPos - 1];
                    if (precedingChar.isLetterOrNumber() || precedingChar == '_') {
                        isWholeWord = false;
                    }
                }

                // Check following character
                int endPos = startPos + whatToSearchFor.length();
                if (endPos < contentToSearchIn.length()) {
                    QChar followingChar = contentToSearchIn[endPos];
                    if (followingChar.isLetterOrNumber() || followingChar == '_') {
                        isWholeWord = false;
                    }
                }

                if (!isWholeWord) {
                    startPos += whatToSearchFor.length();
                    continue;
                }
            }

            SearchMatchResult result;
            result.start = static_cast<uint32_t>(startPos);
            result.length = static_cast<uint32_t>(whatToSearchFor.length());
            results.emplace_back(result);

            startPos += whatToSearchFor.length();
        }
    }

    return results;
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
