#include "QLinkerScriptSession.h"
#include "../MemoryVisualizer/QMemoryVisualizer.h"
#include "../QScintilla/src/Qsci/qsciscintilla.h"
#include "../QScintilla/ComponentHelpers.h"
#include "../../ParsingEngine/CLexer.h"
#include "../../ParsingEngine/CMasterParser.h"
#include "../../DrcEngine/CDrcManager.h"
#include "../../Models/CLinkerScriptContentBase.h"
#include "Components/QChromeTab/QChromeTabWidget.h"
#include "Components/QSearchPopup/QSearchPopup.h"
#include "DrcEngine/CDrcViolation.h"
#include "ParsingEngine/CLexerViolation.h"
#include "ParsingEngine/CParserViolation.h"
#include "ParsingEngine/EParserViolationCode.h"

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
    this->m_drcManager = std::make_unique<CDrcManager>();

    // Setup deferred procedure call system
    QObject::connect(&this->m_deferredProcedureCaller, &QTimer::timeout, this, &QLinkerScriptSession::DeferredContentProcessingAction);
    this->m_deferredProcedureCaller.setSingleShot(true);

    // Connect Scintilla editor signals and activate event filters and setup indicators.
    this->m_scintilla->installEventFilter(this);
    this->m_scintilla->indicatorDefine(QsciScintilla::RoundBoxIndicator, static_cast<int>(Indicators::IndicatorFind));
    this->m_scintilla->setIndicatorOutlineColor(QColor::fromRgb(0, 0, 0, 0), static_cast<int>(Indicators::IndicatorFind));
    this->m_scintilla->setIndicatorForegroundColor(QColor::fromRgb(0xff, 0xff, 0xff, 0x1F), static_cast<int>(Indicators::IndicatorFind));
    QObject::connect(this->m_scintilla, &QsciScintilla::textChanged, this, &QLinkerScriptSession::EditorContentUpdated);
    QObject::connect(this->m_scintilla, &QsciScintilla::SCN_CHARADDED, this, &QLinkerScriptSession::OnCharAddedToEditor);
    QObject::connect(this->m_scintilla, &QsciScintilla::resized, this, &QLinkerScriptSession::OnEditorResize);

    // QObject::connect(this->m_scintilla, &QsciScintilla::handleCharAdded, )

    // Connect search popup signals
    QObject::connect(this->m_searchPopup, &QSearchPopup::evSearchReplaceRequested, this, &QLinkerScriptSession::OnSearchReplaceRequested);

    // Make final calls...
    QScintilla::SetComponentStyles(*this->m_scintilla);
    this->PositionSearchPopup();
    this->SetupViolationsView();
}

void QLinkerScriptSession::SetupViolationsView()
{    
    // Create the QStandardItemModel with 4 columns (1 for item and 3 for additional data)    
    this->m_violationsItemModel.reset(new QStandardItemModel());
    this->m_violationsItemModel->setHorizontalHeaderLabels({ "Code", "Severity", "Description", "Line", "Column", "Offending content"});

    this->m_issuesTreeView->setModel(this->m_violationsItemModel.get());
    this->m_issuesTreeView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);    
    this->m_issuesTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->m_issuesTreeView->setColumnWidth(0, 140);
    this->m_issuesTreeView->setColumnWidth(1, 100);
    this->m_issuesTreeView->setColumnWidth(2, 440);
    this->m_issuesTreeView->setColumnWidth(3, 60);
    this->m_issuesTreeView->setColumnWidth(4, 60);
    this->m_issuesTreeView->setColumnWidth(5, 400);

    this->m_lexerViolationsItem = std::make_shared<QStandardItem>("Lexer Violations");
    this->m_lexerViolationsItem->setSelectable(true);
    this->m_lexerViolationsItem->setEditable(false);
    this->m_parserViolationsItem = std::make_shared<QStandardItem>("Parser Violations");
    this->m_parserViolationsItem->setSelectable(true);
    this->m_parserViolationsItem->setEditable(false);
    this->m_drcViolationsItem = std::make_shared<QStandardItem>("DRC Violations");
    this->m_drcViolationsItem->setSelectable(true);
    this->m_drcViolationsItem->setEditable(false);

    this->m_violationsItemModel->appendRow({ this->m_lexerViolationsItem.get() });
    this->m_violationsItemModel->appendRow({ this->m_parserViolationsItem.get() });
    this->m_violationsItemModel->appendRow({ this->m_drcViolationsItem.get() });
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
    this->m_linkerScriptFile = CLexer::LexLinkerScript(absoluteFilePath, textToLex);
    this->InitiateDeferredProcessing();
}

void QLinkerScriptSession::UpdateDrcViolationsInModel() const
{
    // OK, we have all the violations we need, now update the standard model item        
    this->m_drcViolationsItem->removeRows(0, this->m_lexerViolationsItem->rowCount());

	for (const auto& drcViolation : this->m_linkerScriptFile->DrcViolations())
	{
		auto castDrcViolation = std::dynamic_pointer_cast<CDrcViolation>(drcViolation);
		auto code = new QStandardItem(QString::fromStdString(MapDrcViolationToCode(castDrcViolation->Code())));
		auto description = new QStandardItem(QString::fromStdString(MapDrcViolationToDescription(castDrcViolation->Code())));
        auto severity = new QStandardItem(QString::fromStdString(MapSeverityToString(castDrcViolation->Severity())));
		//auto offendingContent = new QStandardItem(QString::fromStdString(castDrcViolation->OffendingContent()));
		auto offendingContent = new QStandardItem(QString::fromStdString(""));

		QStandardItem* lineNumber;
		QStandardItem* columnIndex;

		if (!castDrcViolation->InvolvedElements().empty())
		{
			if (!castDrcViolation->InvolvedElements().at(0)->RawEntries().empty())
			{
				auto firstEntry = castDrcViolation->InvolvedElements().at(0)->RawEntries().at(0);
				lineNumber = new QStandardItem(QString::number(firstEntry.StartLineNumber()));
				columnIndex = new QStandardItem(QString::number(firstEntry.StartIndexInLine()));
			}
			else
			{
				lineNumber = new QStandardItem(QString::number(0));
				columnIndex = new QStandardItem(QString::number(0));
			}
		}
		else
		{
			lineNumber = new QStandardItem(QString::number(0));
			columnIndex = new QStandardItem(QString::number(0));
		}

		code->setSelectable(false);
		code->setEditable(false);
		lineNumber->setSelectable(false);
		lineNumber->setEditable(false);
		columnIndex->setSelectable(false);
		columnIndex->setEditable(false);
		offendingContent->setSelectable(false);
		offendingContent->setEditable(false);
		this->m_drcViolationsItem->appendRow({ code, severity, description, lineNumber, columnIndex, offendingContent });
	}
}

void QLinkerScriptSession::UpdateParserViolationsInModel() const
{
    // OK, we have all the violations we need, now update the standard model item    
    this->m_parserViolationsItem->removeRows(0, this->m_lexerViolationsItem->rowCount());

	for (const auto& parserViolation : this->m_linkerScriptFile->ParserViolations())
	{
		auto castParserViolation = std::dynamic_pointer_cast<CParserViolation>(parserViolation);
		auto code = new QStandardItem(QString::fromStdString(MapParserViolationToCode(castParserViolation->Code())));
        auto severity = new QStandardItem(QString::fromStdString(MapSeverityToString(castParserViolation->Severity())));
		auto description = new QStandardItem(QString::fromStdString(MapParserViolationToDescription(castParserViolation->Code())));
		//auto offendingContent = new QStandardItem(QString::fromStdString(castParserViolation->OffendingContent()));
		auto offendingContent = new QStandardItem(QString::fromStdString(castParserViolation->GetOffendingContent(this->m_linkerScriptFile->RawFile())));

		QStandardItem* lineNumber;
		QStandardItem* columnIndex;        

		if (!castParserViolation->InvoledEntries().empty())
		{
			lineNumber = new QStandardItem(QString::number(castParserViolation->InvoledEntries().cbegin()->StartLineNumber()));
			columnIndex = new QStandardItem(QString::number(castParserViolation->InvoledEntries().cbegin()->StartIndexInLine()));
		}
		else
		{
			if (castParserViolation->EntryBeforeViolation().EntryType() != RawEntryType::NotPresent)
			{
				lineNumber = new QStandardItem(QString::number(castParserViolation->EntryBeforeViolation().StartLineNumber()));
				columnIndex = new QStandardItem(QString::number(castParserViolation->EntryBeforeViolation().StartIndexInLine()));
			}
			else if (castParserViolation->EntryAfterViolation().EntryType() != RawEntryType::NotPresent)
			{
				lineNumber = new QStandardItem(QString::number(castParserViolation->EntryAfterViolation().StartLineNumber()));
				columnIndex = new QStandardItem(QString::number(castParserViolation->EntryAfterViolation().StartIndexInLine()));
			}
			else
			{
				lineNumber = new QStandardItem(0);
				columnIndex = new QStandardItem(0);
			}            
		}

		code->setSelectable(false);
		code->setEditable(false);
		lineNumber->setSelectable(false);
		lineNumber->setEditable(false);
		columnIndex->setSelectable(false);
		columnIndex->setEditable(false);
		offendingContent->setSelectable(false);
		offendingContent->setEditable(false);
		this->m_parserViolationsItem->appendRow({ code, severity, description, lineNumber, columnIndex, offendingContent });
	}
}

void QLinkerScriptSession::UpdateLexerViolationsInModel() const
{
    // OK, we have all the violations we need, now update the standard model item
    this->m_lexerViolationsItem->removeRows(0, this->m_lexerViolationsItem->rowCount());

	for (const auto& lexerViolation : this->m_linkerScriptFile->LexerViolations())
	{
		auto castLexerViolation = std::dynamic_pointer_cast<CLexerViolation>(lexerViolation);        
		auto code = new QStandardItem(QString::fromStdString(MapLexerViolationToCode(castLexerViolation->Code())));
        auto severity = new QStandardItem(QString::fromStdString(MapSeverityToString(castLexerViolation->Severity())));
		auto description = new QStandardItem(QString::fromStdString(MapLexerViolationToDescription(castLexerViolation->Code())));
		auto lineNumber = new QStandardItem(QString::number(castLexerViolation->ViolationLocation().StartLineNumber()));
		auto columnIndex = new QStandardItem(QString::number(castLexerViolation->ViolationLocation().StartIndexInLine()));
		auto offendingContent = new QStandardItem(QString::fromStdString(this->m_linkerScriptFile->ResolveEntryText(castLexerViolation->ViolationLocation())));

		code->setSelectable(false);
		code->setEditable(false);
		lineNumber->setSelectable(false);
		lineNumber->setEditable(false);
		columnIndex->setSelectable(false);
		columnIndex->setEditable(false);
		offendingContent->setSelectable(false);
		offendingContent->setEditable(false);
		this->m_lexerViolationsItem->appendRow({ code, severity, description, lineNumber, columnIndex, offendingContent });
	}
}

void QLinkerScriptSession::DeferredContentProcessingAction() const
{
	CMasterParser::ParseLinkerScriptFile(this->m_linkerScriptFile);    
    this->m_drcManager->PerformAnalysis(this->m_linkerScriptFile);

    UpdateLexerViolationsInModel();
    UpdateParserViolationsInModel();
    UpdateDrcViolationsInModel();
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

    // TODO: To be implemented in full.
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
