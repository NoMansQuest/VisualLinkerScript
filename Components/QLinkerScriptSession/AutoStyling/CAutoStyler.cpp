#include "CAutoStyler.h"

#include <qlogging.h>

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling;
using namespace VisualLinkerScript::Models::Raw;

SharedPtrVector<SStylerActionBase> CAutoStyler::TextInsertionRequested(
	std::shared_ptr<CRawFile> lexedScript,
	std::string text,
	uint32_t lineNumber,
	uint32_t columnIndex)
{
    /*
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
    */

	return SharedPtrVector<SStylerActionBase>();
}

SharedPtrVector<SStylerActionBase> CAutoStyler::TextRemovalRequested(
	std::shared_ptr<CRawFile> lexedScript,
	uint32_t startLineNumber,
	uint32_t startColumnIndex,
	uint32_t endLineNumber,
	uint32_t endColumnIndex)
{
    return SharedPtrVector<SStylerActionBase>();
}
