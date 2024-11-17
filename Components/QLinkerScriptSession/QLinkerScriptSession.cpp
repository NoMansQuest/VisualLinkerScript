#include "QLinkerScriptSession.h"

#include "../MemoryVisualizer/QMemoryVisualizer.h"
#include "../QScintilla/src/Qsci/qsciscintilla.h"
#include "../QScintilla/ComponentHelpers.h"
#include "../../ParsingEngine/CLinkerScriptLexer.h"
#include "../../ParsingEngine/CLinkerScriptParser.h"
#include "../../DrcEngine/CDrcManager.h"
#include "../../Models/CParsedContentBase.h"
#include "Components/QChromeTab/QChromeTabWidget.h"
#include "Components/QSearchPopup/QSearchPopup.h"
#include "DrcEngine/CDrcViolation.h"
#include "EditorAction/SEditorAddContent.h"
#include "EditorAction/SEditorAddEmptyLine.h"
#include "EditorAction/SEditorRemoveContent.h"
#include "EditorAction/SEditorRemoveLine.h"
#include "EditorAction/SEditorReplaceContent.h"
#include "EditorAction/SEditorSelectText.h"
#include "EditorAction/SEditorSetCaretPosition.h"
#include "EditorAction/SEditorSetLineContent.h"
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
using namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction;

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

static QIcon GetIconForSeverity(const ESeverityCode severity)
{
    switch (severity)
    {
	    case ESeverityCode::Error:
	        return QIcon(":/resources/Images/error-sign-icon.svg");
	    case ESeverityCode::Warning:
	        return QIcon(":/resources/Images/warning-sign-icon.svg");
	    case ESeverityCode::Information:
	        return QIcon(":/resources/Images/info-sign-icon.svg");
    	default:
            throw std::exception("Unrecognized severity");
    }
}

uint32_t QLinkerScriptSession::EditorLineNumber() const
{
    int line, column;
    this->m_scintilla->getCursorPosition(&line, &column);
    return line + 1;
}

uint32_t QLinkerScriptSession::EditorTotalLines() const
{
    return static_cast<uint32_t>(this->m_scintilla->lines());
}

uint32_t QLinkerScriptSession::EditorColumnIndex() const
{
    int line, column;
    this->m_scintilla->getCursorPosition(&line, &column);
    return column;
}

uint32_t QLinkerScriptSession::EditorAbsolutePosition() const
{
    int line, column;
    this->m_scintilla->getCursorPosition(&line, &column);
    return static_cast<uint32_t>(this->m_scintilla->positionFromLineIndex(line, column));
}


void QLinkerScriptSession::BuildUserInterface()
{
    // Setup UI.
    this->m_issuesTreeView = new QIssuesTreeView(this);
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
    QObject::connect(this->m_scintilla, &QsciScintilla::cursorPositionChanged, this, &QLinkerScriptSession::OnEditorCursorPositionChanged);

    // Connect search popup signals
    QObject::connect(this->m_searchPopup, &QSearchPopup::evSearchReplaceRequested, this, &QLinkerScriptSession::OnSearchReplaceRequested);

    // Make final calls...    
    this->PositionSearchPopup();
    this->SetupViolationsView();
    this->SetupEditor();
}

void QLinkerScriptSession::SetupEditor()
{
    QScintilla::SetComponentStyles(*this->m_scintilla);

    // Setup indicators
    this->m_scintilla->indicatorDefine(QsciScintilla::SquiggleLowIndicator, static_cast<int>(Indicators::IndicatorParserViolation));
    this->m_scintilla->indicatorDefine(QsciScintilla::SquiggleLowIndicator, static_cast<int>(Indicators::IndicatorDrcViolation));
    this->m_scintilla->indicatorDefine(QsciScintilla::SquiggleLowIndicator, static_cast<int>(Indicators::IndicatorLexerViolation));

    // Setup indicators style
    this->m_scintilla->setIndicatorForegroundColor(QColor::fromRgb(255, 0, 0), static_cast<int>(Indicators::IndicatorParserViolation));
    this->m_scintilla->setIndicatorForegroundColor(QColor::fromRgb(255, 102, 255), static_cast<int>(Indicators::IndicatorLexerViolation));
    this->m_scintilla->setIndicatorForegroundColor(QColor::fromRgb(255, 153, 51), static_cast<int>(Indicators::IndicatorParserViolation));

    // Setup selection style
    this->m_scintilla->setSelectionBackgroundColor(QColor::fromRgb(48, 31, 99));
    this->m_scintilla->setSelectionForegroundColor(QColor::fromRgb(255, 255, 255));
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

    this->m_issuesTreeView->Initialize();
}

void QLinkerScriptSession::OnEditorResize() const
{
    this->PositionSearchPopup();
}

void QLinkerScriptSession::OnEditorCursorPositionChanged(int line, int column)
{
    emit this->evEditorCursorChanged(this->m_sessionId);
}

void QLinkerScriptSession::UpdateContent(const std::string& newContent) const
{
    this->m_scintilla->setText(QString::fromStdString(newContent));
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

            for (const auto& entry: foundEntries)
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

std::shared_ptr<CLinkerScriptFile> QLinkerScriptSession::LinkerScriptFile() const
{
    return this->m_linkerScriptFile;
}    

void QLinkerScriptSession::EditorContentUpdated()
{
    this->m_linkerScriptFile->UpdateContent(this->m_scintilla->text().toStdString());
    CLinkerScriptLexer::LexLinkerScript(this->m_linkerScriptFile);

    // The deferred processing involves parsing and DRC.
    this->InitiateDeferredProcessing();
}

void QLinkerScriptSession::UpdateDrcViolationsInModel() const
{
    // OK, we have all the violations we need, now update the standard model item        
    this->m_drcViolationsItem->removeRows(0, this->m_drcViolationsItem->rowCount());
    this->m_scintilla->clearIndicatorRange(0, this->m_scintilla->length() - 1, static_cast<int>(Indicators::IndicatorDrcViolation));

	for (const auto& drcViolation : this->m_linkerScriptFile->DrcViolations())
	{
		const auto castDrcViolation = std::dynamic_pointer_cast<CDrcViolation>(drcViolation);
        const auto code = MapDrcViolationToCode(castDrcViolation->Code());
        const auto severity = castDrcViolation->Severity();
        const auto description = MapDrcViolationToCode(castDrcViolation->Code());
        const auto offendingContent = ""; // TODO: To be fixed.
        uint32_t lineNumber = 0;
        uint32_t columnIndex = 0;

		if (!castDrcViolation->InvolvedElements().empty() && !castDrcViolation->InvolvedElements().at(0)->RawEntries().empty())
		{
			const auto& firstEntry = castDrcViolation->InvolvedElements().at(0)->RawEntries().at(0);
            lineNumber = firstEntry.StartLineNumber();
			columnIndex = firstEntry.StartIndexInLine();			
		}

        AddViolation(*this->m_parserViolationsItem, code, severity, description, lineNumber, columnIndex, offendingContent);
	}
}

void QLinkerScriptSession::UpdateParserViolationsInModel() const
{
    // OK, we have all the violations we need, now update the standard model item    
    this->m_parserViolationsItem->removeRows(0, this->m_parserViolationsItem->rowCount());
    this->m_scintilla->clearIndicatorRange(0, this->m_scintilla->length() - 1, static_cast<int>(Indicators::IndicatorParserViolation));

	for (const auto& parserViolation : this->m_linkerScriptFile->ParserViolations())
	{
		const auto castParserViolation = std::dynamic_pointer_cast<CParserViolation>(parserViolation);
        const auto code = MapParserViolationToCode(castParserViolation->Code());
        const auto severity = castParserViolation->Severity();
        const auto description = MapParserViolationToDescription(castParserViolation->Code());
        auto offendingContent = castParserViolation->GetOffendingContent(this->m_linkerScriptFile);
        uint32_t lineNumber = 0;
		uint32_t columnIndex = 0;

		if (!castParserViolation->InvoledEntries().empty())
		{
			lineNumber = castParserViolation->InvoledEntries().cbegin()->StartLineNumber();
			columnIndex = castParserViolation->InvoledEntries().cbegin()->StartIndexInLine();
		}
		else
		{
			if (castParserViolation->EntryBeforeViolation().EntryType() != RawEntryType::NotPresent)
			{
				lineNumber = castParserViolation->EntryBeforeViolation().StartLineNumber();
				columnIndex = castParserViolation->EntryBeforeViolation().StartIndexInLine();
			}
			else if (castParserViolation->EntryAfterViolation().EntryType() != RawEntryType::NotPresent)
			{
				lineNumber = castParserViolation->EntryAfterViolation().StartLineNumber();
				columnIndex = castParserViolation->EntryAfterViolation().StartIndexInLine();
			}      
		}

        AddViolation(*this->m_parserViolationsItem, code, severity, description, lineNumber, columnIndex, offendingContent);

        if (!castParserViolation->InvoledEntries().empty())
        {
            for (const auto& entryToHighlight: castParserViolation->InvoledEntries())
            {
                this->m_scintilla->fillIndicatorRange(
                    entryToHighlight.StartLineNumber(),
                    entryToHighlight.StartIndexInLine(),
                    entryToHighlight.EndLineNumber(),
                    entryToHighlight.EndIndexInLine(),
                    static_cast<int>(Indicators::IndicatorParserViolation));
            }
        }
        else
        {
			if (castParserViolation->EntryBeforeViolation().IsPresent())
			{
                const auto entryToStyle = castParserViolation->EntryBeforeViolation();
                this->m_scintilla->fillIndicatorRange(
                    entryToStyle.StartLineNumber(),
                    entryToStyle.StartIndexInLine(),
                    entryToStyle.EndLineNumber(),
                    entryToStyle.EndIndexInLine(),
                    static_cast<int>(Indicators::IndicatorParserViolation));
			}
            else if (castParserViolation->EntryAfterViolation().IsPresent())
            {
                const auto entryToStyle = castParserViolation->EntryBeforeViolation();
                this->m_scintilla->fillIndicatorRange(
                    entryToStyle.StartLineNumber(),
                    entryToStyle.StartIndexInLine(),
                    entryToStyle.EndLineNumber(),
                    entryToStyle.EndIndexInLine(),
                    static_cast<int>(Indicators::IndicatorParserViolation));
            }
        }
	}
}

void QLinkerScriptSession::UpdateLexerViolationsInModel() const
{
    // OK, we have all the violations we need, now update the standard model item
    this->m_lexerViolationsItem->removeRows(0, this->m_lexerViolationsItem->rowCount());
    this->m_scintilla->clearIndicatorRange(0, this->m_scintilla->length() - 1, static_cast<int>(Indicators::IndicatorLexerViolation));

	for (const auto& lexerViolation : this->m_linkerScriptFile->LexerViolations())
	{
        const auto castLexerViolation = std::dynamic_pointer_cast<CLexerViolation>(lexerViolation);
        const auto code = MapLexerViolationToCode(castLexerViolation->Code());
        const auto severity = castLexerViolation->Severity();
        const auto description = MapLexerViolationToDescription(castLexerViolation->Code());
        const auto lineNumber = castLexerViolation->ViolationLocation().StartLineNumber();
        const auto columnIndex = castLexerViolation->ViolationLocation().StartIndexInLine();
		const auto offendingContent = this->m_linkerScriptFile->ResolveRawEntry(castLexerViolation->ViolationLocation());
        AddViolation(*this->m_lexerViolationsItem, code, severity, description, lineNumber, columnIndex, offendingContent);

        const auto entryToStyle = castLexerViolation->ViolationLocation();
        this->m_scintilla->fillIndicatorRange(
            entryToStyle.StartLineNumber(), 
            entryToStyle.StartIndexInLine(),
            entryToStyle.EndLineNumber(), 
            entryToStyle.EndIndexInLine(), 
            static_cast<int>(Indicators::IndicatorLexerViolation));        
	}
}

void QLinkerScriptSession::AddViolation(
    QStandardItem& rootItem,
    const std::string& code,
    const ESeverityCode severity,
    const std::string& description,
    const uint32_t lineNumber,
    const uint32_t columnIndex,
    const std::string& offendingCode)
{
    auto codeItem = new QStandardItem(GetIconForSeverity(severity), QString::fromStdString(code));
    auto severityItem = new QStandardItem(QString::fromStdString(MapSeverityToString(severity)));
    auto descriptionItem = new QStandardItem(QString::fromStdString(description));
    auto lineNumberItem = new QStandardItem(QString::number(lineNumber));
    auto columnIndexItem = new QStandardItem(QString::number(columnIndex));
    auto offendingContentItem = new QStandardItem(QString::fromStdString(offendingCode));

    codeItem->setSelectable(true);
    codeItem->setEditable(false);
    lineNumberItem->setSelectable(true);
    lineNumberItem->setEditable(false);
    columnIndexItem->setSelectable(true);
    columnIndexItem->setEditable(false);
    descriptionItem->setSelectable(true);
    descriptionItem->setEditable(false);
    severityItem->setSelectable(true);
    severityItem->setEditable(false);
    offendingContentItem->setSelectable(true);
    offendingContentItem->setEditable(false);
    rootItem.appendRow({ codeItem, severityItem, descriptionItem, lineNumberItem, columnIndexItem, offendingContentItem });
}

void QLinkerScriptSession::DeferredContentProcessingAction()
{
    // TODO: Candidate for optimization (i.e. m_scintilla->text().toStdString() results in a 'copy').    
	CLinkerScriptParser::ParseLinkerScriptFile(this->m_linkerScriptFile);    
    this->m_drcManager.PerformAnalysis(this->m_linkerScriptFile);

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
        if (obj == this->m_scintilla)
        {
            // Special case: For 'Escape', we perform some actions before
            // processing the key-press action.
            if (keyEvent->key() == Qt::Key_Escape)
            {
                this->m_searchPopup->hide();
                this->ResetFindIndicators(Indicators::IndicatorFind);
                this->m_scintilla->setSelection(0, 0, 0, 0);                
                this->m_searchSessionActive = false;
                this->m_searchSessionCurrentFocusedEntryStartPosition = false;
            }
            
            if (this->m_scintilla->getSelectionsCount() > 1)
            {
                return QWidget::eventFilter(obj, event);
            }

            // We need to Lex the content before handing over controller to AutoStyler, as the styler
            // needs access to an up-to-date content.
            this->m_linkerScriptFile->UpdateContent(this->m_scintilla->text().toStdString());
            CLinkerScriptLexer::LexLinkerScript(this->m_linkerScriptFile);

            // Make further calls as needed.
            int lineNumber, columnIndex;
            this->m_scintilla->getCursorPosition(&lineNumber, &columnIndex);
            const auto absolutePosition = this->m_scintilla->positionFromLineIndex(lineNumber, columnIndex);
            const auto actionsToPerform = this->m_autoStyler.ProcessKeyboardEvent(
                this->m_linkerScriptFile,
                *keyEvent,
                absolutePosition,
                lineNumber,
                columnIndex);

            // In case there are actions to perform, we'll exit here.
            if (!actionsToPerform.empty())
            {
                this->ApplyEditorActions(actionsToPerform);
                return true;
            }            
        }	    
    }    
    return QWidget::eventFilter(obj, event);
}

void QLinkerScriptSession::ApplyEditorActions(SharedPtrVector<SEditorActionBase> actions) const
{
    this->m_scintilla->beginUndoAction();

    std::sort(
        actions.begin(), 
        actions.end(), 
        [](const std::shared_ptr<SEditorActionBase>& a, const std::shared_ptr<SEditorActionBase>& b)
	    {
            return (a->LineNumber == b->LineNumber)
        		? a->ColumnIndex < b->ColumnIndex
        		: a->LineNumber < b->LineNumber;
	    });

	for (const auto& action : actions)
	{
        switch (action->ActionType())
        {
		    case EEditorActionType::AddContent:
		    {
                const auto castAddContent = std::dynamic_pointer_cast<SEditorAddContent>(action);
                this->m_scintilla->insertAt(
                    QString::fromStdString(castAddContent->TextToInsert), 
                    castAddContent->LineNumber, 
                    castAddContent->ColumnIndex);
		        break;
		    }

            case EEditorActionType::AddEmptyLine:
            {
                const auto castAddEmptyLine = std::dynamic_pointer_cast<SEditorAddEmptyLine>(action);
                this->m_scintilla->insertAt("\n", castAddEmptyLine->LineNumber, this->m_scintilla->lineLength(castAddEmptyLine->LineNumber));
                break;
            }

        	case EEditorActionType::SetLineContent:
            {
                const auto castSetLineContent = std::dynamic_pointer_cast<SEditorSetLineContent>(action);
                this->m_scintilla->setLineText(QString::fromStdString(castSetLineContent->TextToSet), castSetLineContent->LineNumber);
                break;
            }

            case EEditorActionType::RemoveContent:
            {
                const auto castRemoveContent = std::dynamic_pointer_cast<SEditorRemoveContent>(action);
                this->m_scintilla->deleteRange(
                    castRemoveContent->LineNumber,
                    castRemoveContent->ColumnIndex, 
                    castRemoveContent->EndLineNumber, 
                    castRemoveContent->EndColumnIndex);
                break;
            }

            case EEditorActionType::RemoveLine:
            {
                const auto castRemoveLine    = std::dynamic_pointer_cast<SEditorRemoveLine>(action);
                const auto endColumnIndex = this->m_scintilla->lineLength(castRemoveLine->LineNumber) - 1;
                this->m_scintilla->deleteRange(castRemoveLine->LineNumber,0,castRemoveLine->LineNumber, endColumnIndex);
                break;
            }

            case EEditorActionType::ReplaceContent:
            {
                const auto castReplaceContent = std::dynamic_pointer_cast<SEditorReplaceContent>(action);
                
                this->m_scintilla->deleteRange(
                    castReplaceContent->LineNumber,
                    castReplaceContent->ColumnIndex,
                    castReplaceContent->EndLineNumber,
                    castReplaceContent->EndColumnIndex);

                this->m_scintilla->insertAt(
                    QString::fromStdString(castReplaceContent->TextToReplace),
                    castReplaceContent->LineNumber,
                    castReplaceContent->ColumnIndex);

                break;
            }

            case EEditorActionType::SelectText:
            {
                const auto castSelectText = std::dynamic_pointer_cast<SEditorSelectText>(action);
                this->m_scintilla->setSelection(
                    castSelectText->LineNumber,
                    castSelectText->ColumnIndex,
                    castSelectText->EndLineNumber,
                    castSelectText->EndColumnIndex);
                break;
            }

        	case EEditorActionType::SetCaretPosition:
            {
                const auto castSetCaretPosition = std::dynamic_pointer_cast<SEditorSetCaretPosition>(action);
                this->m_scintilla->setCursorPosition(castSetCaretPosition->LineNumber, castSetCaretPosition->ColumnIndex);
                break;
            }

        	default:
                throw std::exception("Unrecognized editor action type.");
        }
	}
    this->m_scintilla->endUndoAction();
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

void QLinkerScriptSession::JumpToLine(uint32_t lineNumber) const
{
    if (static_cast<int>(lineNumber - 1) >= this->m_scintilla->lines())
    {
        return;
    }

    this->m_scintilla->setSelection(
        static_cast<int>(lineNumber) - 1, 0,
        static_cast<int>(lineNumber) - 1, 0);
    this->m_scintilla->ensureLineVisible(static_cast<int>(lineNumber));
    this->m_scintilla->setFocus(Qt::FocusReason::OtherFocusReason);
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
};