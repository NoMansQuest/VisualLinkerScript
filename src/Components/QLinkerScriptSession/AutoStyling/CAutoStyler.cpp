#include "Components/QLinkerScriptSession/AutoStyling/CAutoStyler.h"
#include "Models/CLinkerScriptFile.h"
#include "Models/Raw/CRawEntry.h"
#include <qlogging.h>
#include <stack>

#include "Components/QLinkerScriptSession/EditorAction/SEditorAddContent.h"
#include "Components/QLinkerScriptSession/EditorAction/SEditorAddEmptyLine.h"
#include "Components/QLinkerScriptSession/EditorAction/SEditorSetCaretPosition.h"
#include "Components/QLinkerScriptSession/EditorAction/SEditorSetLineContent.h"

const int TabSize = 4;

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Models::Raw;
using namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling;
using namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction;

bool HasNonWhitespaceBeforeCurlyBracket(const std::string& str, uint32_t backwardStartingIndex);

void ProcessLineFeed(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, 
    const uint32_t absolutePosition, 
    const uint32_t lineNumber, 
    const uint32_t columnIndex, 
    LinqVector<SEditorActionBase>& actionsToPerform);

CRawEntry PositionDropTest(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, 
    const uint32_t absoluteCharacterPosition);

CRawEntry PrecedingBracketOpen(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
    const uint32_t absoluteCharacterPosition);

CRawEntry PrecedingBracketOpenOnLine(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, 
    uint32_t line, 
    const uint32_t absoluteCharacterPosition);

CRawEntry PrecedingImmediateBracketOpenOnLine(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
    uint32_t line,
    const uint32_t absoluteCharacterPosition);

CRawEntry SucceedingBracketClose(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
    const uint32_t absoluteCharacterPosition);

CRawEntry SucceedingBracketCloseOnLine(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, 
    uint32_t line, 
    const uint32_t absoluteCharacterPosition);

CRawEntry SucceedingImmediateBracketCloseOnLine(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
    uint32_t line,
    const uint32_t absoluteCharacterPosition);

uint32_t LeadingWhiteSpaces(const std::string& stringToInspect);

LinqVector<SEditorActionBase> CAutoStyler::ProcessKeyboardEvent(
	const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
    const QKeyEvent& keyEvent,
	const uint32_t absolutePosition,
    const uint32_t lineNumber,
    const uint32_t columnIndex)
{
    LinqVector<SEditorActionBase> actionsToPerform;
    const auto entryContainingCharacter = PositionDropTest(linkerScriptFile, absolutePosition);

    switch (keyEvent.key())
    {
    	case Qt::Key_Return:
    	case Qt::Key_Enter:
    	{
            ProcessLineFeed(linkerScriptFile, absolutePosition, lineNumber, columnIndex, actionsToPerform);
            break;
    	}

    	case Qt::Key_ParenLeft: // '('
    	{
            const auto insertAction = std::make_shared<SEditorAddContent>(lineNumber, columnIndex, std::string("()"));
            const auto moveCursorAction = std::make_shared<SEditorSetCaretPosition>(lineNumber, columnIndex + 1);
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(insertAction));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(moveCursorAction));
            break;
    	}

        case Qt::Key_BraceLeft: // '{'
        {
            const auto insertAction = std::make_shared<SEditorAddContent>(lineNumber, columnIndex, std::string("{}"));
            const auto moveCursorAction = std::make_shared<SEditorSetCaretPosition>(lineNumber, columnIndex + 1);
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(insertAction));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(moveCursorAction));
            break;
        }

        case Qt::Key_BraceRight: // '}'
        {
            /*
            if (HasNonWhitespaceBeforeCurlyBracket(textAtLine.toStdString(), index - 1))
	        {
	            return; // No action is taken.
	        }
	        const auto newLineContent = std::string(4 * indentationData.IndentationDepth(), ' ') + "}";
	        this->m_scintilla->setLineText(QString::fromStdString(newLineContent), lineNumber);
	        this->m_scintilla->setCursorPosition(lineNumber, newLineContent.length());
            */
            break;
        }

        case Qt::Key_QuoteDbl: // Double quotation (")
        {
            const auto insertAction = std::make_shared<SEditorAddContent>(lineNumber, columnIndex, std::string("\"\""));
            const auto moveCursorAction = std::make_shared<SEditorSetCaretPosition>(lineNumber, columnIndex + 1);
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(insertAction));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(moveCursorAction));
            break;
        }

    	default:
            break;
    }
       

	return actionsToPerform;
}

void ProcessLineFeed(
    const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
    const uint32_t absolutePosition,
    const uint32_t lineNumber,
    const uint32_t columnIndex,
    LinqVector<SEditorActionBase>& actionsToPerform)
{
    // Edge case: In case of no text present, no action is to be undertaken.
    if (linkerScriptFile->Content().empty())
    {
        return;
    }

    const auto textAtCurrentLine = linkerScriptFile->Lines().size() > lineNumber ? linkerScriptFile->Lines().at(lineNumber) : std::string("");
    const auto encapsulatingEntry = PositionDropTest(linkerScriptFile, absolutePosition);
    const auto indentationData = linkerScriptFile->IndentationData()[lineNumber];    

    if (encapsulatingEntry.EntryType() == RawEntryType::Comment)
    {
        auto newLineContent = "\n" + std::string(TabSize * indentationData.IndentationDepth(), ' ');
        if (indentationData.CommentLinePrefixing())
        {
            newLineContent += " *";
        }
        const auto insertAction = std::make_shared<SEditorAddContent>(lineNumber, columnIndex, newLineContent);
        const auto moveCursorAction = std::make_shared<SEditorSetCaretPosition>(lineNumber + 1, newLineContent.length() - 1);
        actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(insertAction));
        actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(moveCursorAction));
    }
    else if (encapsulatingEntry.EntryType() == RawEntryType::String)
    {
        auto newLineContent = "\n" + std::string(TabSize * indentationData.IndentationDepth(), ' ');
        if (indentationData.CommentLinePrefixing())
        {
            newLineContent += " *";
        }

        const auto insertAction = std::make_shared<SEditorAddContent>(lineNumber, columnIndex, newLineContent);
        const auto moveCursorAction = std::make_shared<SEditorSetCaretPosition>(lineNumber + 1, newLineContent.length() - 1);
        actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(insertAction));
        actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(moveCursorAction));
    }
    else if (!encapsulatingEntry.IsPresent())
    {
        // NOTE: When we receive the next-line character, the text is already going to be broken between two lines.
        //       That has to be considered.
        // Note: The current line is the line post new-line character insertion.
        const auto sameLineBracketOpen = PrecedingBracketOpenOnLine(linkerScriptFile, lineNumber, absolutePosition);
        const auto sameLineBracketClose = SucceedingBracketCloseOnLine(linkerScriptFile, lineNumber, absolutePosition);
        
        if (sameLineBracketOpen.IsPresent() && sameLineBracketClose.IsPresent())
        {
            // Get immediate succeeding or preceding bracket opens
            const auto newLinePadding = sameLineBracketOpen.IsPresent() ? std::string(sameLineBracketOpen.StartIndexInLine() + TabSize, ' ') : "";
            const auto immediateBracketOpen = PrecedingImmediateBracketOpenOnLine(linkerScriptFile, lineNumber, absolutePosition);
            const auto immediateBracketClose = SucceedingImmediateBracketCloseOnLine(linkerScriptFile, lineNumber, absolutePosition);            

            if (immediateBracketOpen.IsPresent() || immediateBracketClose.IsPresent())
            {
                // We're positioned just after bracket overture or just before bracket closure. Apply the whole game.
                const auto contentLength = sameLineBracketClose.StartPosition() - sameLineBracketOpen.StartPosition() - 1;
                const auto encapsulatingText = linkerScriptFile->Content().substr(sameLineBracketOpen.StartPosition() + 1, contentLength);

                const auto bracketClosePadding = std::string(sameLineBracketOpen.StartIndexInLine(), ' ');                
                const auto bracketOpenLineText = StringRTrim(linkerScriptFile->Lines().at(sameLineBracketOpen.StartLineNumber()).substr(0, sameLineBracketOpen.StartIndexInLine() + 1));
                const auto trimmedContent = StringTrim(textAtCurrentLine.substr(sameLineBracketOpen.StartIndexInLine() + 1, sameLineBracketClose.StartIndexInLine() - sameLineBracketOpen.StartIndexInLine() - 1));
                const auto newLineText = newLinePadding + trimmedContent;
                const auto bracketCloseText = bracketClosePadding + StringLTrim(textAtCurrentLine.substr(sameLineBracketClose.StartIndexInLine()));

                auto addLineAction = std::make_shared<SEditorAddEmptyLine>(lineNumber, 0); // We'll use this twice
                auto setBracketOpenLineText = std::make_shared<SEditorSetLineContent>(lineNumber, 0, bracketOpenLineText);
                auto setNewLineText = std::make_shared<SEditorSetLineContent>(lineNumber + 1, 0, newLineText);
                auto setBracketCloseLineText = std::make_shared<SEditorSetLineContent>(lineNumber + 2, 0, bracketCloseText);

                auto newCaretPosition =newLineText.length() - 1;
                auto setCaretPositionAction = std::make_shared<SEditorSetCaretPosition>(lineNumber + 1, newCaretPosition);

                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(addLineAction));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(addLineAction));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setBracketOpenLineText));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setNewLineText));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setBracketCloseLineText));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setCaretPositionAction));                
            }
            else
            {
	            // We're somewhere in the middle of the code block. Only apply indentation
                const auto bracketOpenLineText = StringRTrim(linkerScriptFile->Lines().at(sameLineBracketOpen.StartLineNumber()).substr(0, sameLineBracketOpen.StartIndexInLine() + 1));
                const auto newLineText = newLinePadding + StringRTrim(textAtCurrentLine).substr(columnIndex);

                auto addLineAction = std::make_shared<SEditorAddEmptyLine>(lineNumber, 0); 
                auto setBracketOpenLineText = std::make_shared<SEditorSetLineContent>(lineNumber, 0, bracketOpenLineText);
                auto setNewLineText = std::make_shared<SEditorSetLineContent>(lineNumber + 1, 0, newLineText);
                auto setCaretPositionAction = std::make_shared<SEditorSetCaretPosition>(lineNumber + 1, newLinePadding.length());

                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(addLineAction));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setBracketOpenLineText));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setNewLineText));
                actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setCaretPositionAction));
            }
        }
        else if (sameLineBracketOpen.IsPresent())
        {
            const auto newLinePadding = sameLineBracketOpen.IsPresent() ? std::string(sameLineBracketOpen.StartIndexInLine() + TabSize, ' ') : "";
            const auto trimmedContent = StringTrim(textAtCurrentLine.substr(sameLineBracketOpen.StartIndexInLine() + 1));
            const auto newLineText = newLinePadding + trimmedContent;
            const auto bracketOpenLineText = StringRTrim(linkerScriptFile->Lines().at(sameLineBracketOpen.StartLineNumber()).substr(0, sameLineBracketOpen.StartIndexInLine() + 1));

            auto addLineAction = std::make_shared<SEditorAddEmptyLine>(lineNumber, 0);
            auto setBracketOpenLineText = std::make_shared<SEditorSetLineContent>(lineNumber, 0, bracketOpenLineText);
            auto setNewLineText = std::make_shared<SEditorSetLineContent>(lineNumber + 1, 0, newLineText);
            auto setCaretPositionAction = std::make_shared<SEditorSetCaretPosition>(lineNumber + 1, newLineText.length());
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(addLineAction));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setBracketOpenLineText));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setNewLineText));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setCaretPositionAction));
        }
        else if (sameLineBracketClose.IsPresent())
        {
            /*
            auto leadingWhiteSpaceCount = LeadingWhiteSpaces(textAtLine.toStdString());
            auto minimumWhiteSpaceCount = (4 * (indentationData.IndentationDepth()));
            auto whiteSpacesToAdd = leadingWhiteSpaceCount > minimumWhiteSpaceCount ? 0 : minimumWhiteSpaceCount - leadingWhiteSpaceCount;
            auto newLinePadding = std::string(whiteSpacesToAdd > 0 ? whiteSpacesToAdd : 0, ' ');
            this->m_scintilla->insertAt(QString::fromStdString(newLinePadding), lineNumber, 0);
            this->m_scintilla->setCursorPosition(lineNumber, newLinePadding.length());*/
        }
        else
        {
            // Neither bracket-open nor bracket-close is present on the same line.
            // We need to find the previous bracket-open to get the padding info.
            const auto precedingBracketOpen = PrecedingBracketOpen(linkerScriptFile, absolutePosition);
            const auto trimmedContent = StringTrim(textAtCurrentLine.substr(columnIndex));
            const auto newLineText = (precedingBracketOpen.IsPresent() ? std::string(precedingBracketOpen.StartIndexInLine() + TabSize, ' ') : "") + trimmedContent;
            const auto contentStayingInLine = StringRTrim(textAtCurrentLine.substr(0, columnIndex));

            auto addLineAction = std::make_shared<SEditorAddEmptyLine>(lineNumber, 0);
            auto setSameLineText = std::make_shared<SEditorSetLineContent>(lineNumber, 0, contentStayingInLine);
            auto setNewLineText = std::make_shared<SEditorSetLineContent>(lineNumber + 1, 0, newLineText);
            auto setCaretPositionAction = std::make_shared<SEditorSetCaretPosition>(lineNumber + 1, newLineText.length());
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(addLineAction));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setSameLineText));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setNewLineText));
            actionsToPerform.emplace_back(std::static_pointer_cast<SEditorActionBase>(setCaretPositionAction));
        }
    }
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

CRawEntry PositionDropTest(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, const uint32_t absoluteCharacterPosition)
{
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if ((absoluteCharacterPosition > entry.StartPosition() + 1) && absoluteCharacterPosition < entry.StartPosition() + entry.Length())
        {
            return entry;
        }
    }

    return CRawEntry(); // Not-Present entry (i.e. nothing found).
}

CRawEntry PrecedingBracketOpen(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,const uint32_t absoluteCharacterPosition)
{
    CRawEntry candidate;
    std::stack<CRawEntry> bracketStack;
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if (entry.StartPosition() < absoluteCharacterPosition)
        {
            if (entry.EntryType() == RawEntryType::BracketOpen)
            {
                bracketStack.push(entry);
            }
            else if (entry.EntryType() == RawEntryType::BracketClose)
            {
                if (!bracketStack.empty())
                {
                    bracketStack.pop();
                }
            }
        }        

        if (entry.StartPosition() > absoluteCharacterPosition)
        {
            break;
        }
    }

    return !bracketStack.empty() ? bracketStack.top() : CRawEntry();
}


CRawEntry PrecedingBracketOpenOnLine(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, uint32_t line, const uint32_t absoluteCharacterPosition)
{
    CRawEntry candidate;
    std::stack<CRawEntry> bracketStack;
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if (entry.StartPosition() < absoluteCharacterPosition &&
            entry.StartLineNumber() == line)
        {
            if (entry.EntryType() == RawEntryType::BracketOpen)
            {
                bracketStack.push(entry);
            }
            else if (entry.EntryType() == RawEntryType::BracketClose)
            {
                if (!bracketStack.empty())
                {
                    bracketStack.pop();
                }
            }
        }

        if (entry.StartPosition() > absoluteCharacterPosition)
        {
            break;
        }
    }

    return !bracketStack.empty() ? bracketStack.top() : CRawEntry();
}

CRawEntry PrecedingImmediateBracketOpenOnLine(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, uint32_t line, const uint32_t absoluteCharacterPosition)
{
    CRawEntry candidate;
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if (entry.StartLineNumber() == line && entry.StartPosition() < absoluteCharacterPosition)
        {
            candidate = (entry.EntryType() == RawEntryType::BracketOpen) ? entry : CRawEntry();
        }

        if (entry.StartPosition() > absoluteCharacterPosition)
        {
            break;
        }
    }

    return candidate;
}

CRawEntry SucceedingBracketCloseOnLine(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, uint32_t line, const uint32_t absoluteCharacterPosition)
{
    std::stack<CRawEntry> entriesStack;
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if (entry.StartLineNumber() == line && entry.StartPosition() >= absoluteCharacterPosition)
        {
            if (entry.EntryType() == RawEntryType::BracketOpen)
            {
                entriesStack.push(entry);
            }
            else if (entry.EntryType() == RawEntryType::BracketClose)
            {
                if (!entriesStack.empty())
                {
                    entriesStack.pop();
                }
                else
                {
                    return entry;
                }
            }
        }

        if (entry.StartLineNumber() > line)
        {
            break;
        }
    }

    return CRawEntry();
}

CRawEntry SucceedingBracketClose(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, const uint32_t absoluteCharacterPosition)
{
    std::stack<CRawEntry> entriesStack;
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if (entry.StartPosition() >= absoluteCharacterPosition)
        {
            if (entry.EntryType() == RawEntryType::BracketOpen)
            {
                entriesStack.push(entry);
            }
            else if (entry.EntryType() == RawEntryType::BracketClose)
            {
                if (!entriesStack.empty())
                {
                    entriesStack.pop();
                }
                else
                {
                    return entry;
                }
            }
        }
    }

    return CRawEntry();
}

CRawEntry SucceedingImmediateBracketCloseOnLine(const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile, uint32_t line, const uint32_t absoluteCharacterPosition)
{
    for (const auto& entry : linkerScriptFile->LexedContent())
    {
        if (entry.StartLineNumber() == line && entry.StartPosition() >= absoluteCharacterPosition)
        {
	        if (entry.EntryType() == RawEntryType::BracketClose)
	        {
                return entry;
	        }
            return CRawEntry();
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
