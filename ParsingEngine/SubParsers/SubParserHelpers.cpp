#include <vector>
#include <string>

#include "SubParserHelpers.h"
#include "../../Helpers.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

namespace
{
    bool CanBePartOfWildcard(const CLinkerScriptFile& linkerScriptFile, const CRawEntry& rawEntry)
    {
        switch (rawEntry.EntryType())
        {
            case RawEntryType::Word:
            case RawEntryType::Wildcard:
            case RawEntryType::QuestionMark:
            case RawEntryType::Number:
            {
                return true;
            }

            case RawEntryType::ArithmeticOperator:
            {
                auto resolvedContent = linkerScriptFile.ResolveRawEntry(rawEntry);
                return (resolvedContent == "*") || (resolvedContent == "-") || (resolvedContent == "/");
            }

            case RawEntryType::EvaluativeOperators:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Comma:
            case RawEntryType::Semicolon:
            case RawEntryType::Colon:
            case RawEntryType::String:
            case RawEntryType::Comment:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            case RawEntryType::NotPresent:
            default:
                return false;
        }
    }

    bool CanBeStartOfWildcard(const CLinkerScriptFile& linkerScriptFile, const CRawEntry& rawEntry)
    {
        switch (rawEntry.EntryType())
        {
            case RawEntryType::Word:
            case RawEntryType::Wildcard:
            case RawEntryType::QuestionMark:
            case RawEntryType::Number:
            {
                return true;
            }

            case RawEntryType::ArithmeticOperator:
            {
                auto resolvedContent = linkerScriptFile.ResolveRawEntry(rawEntry);
                return (resolvedContent == "*") || (resolvedContent == "/");
            }

            case RawEntryType::EvaluativeOperators:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Comma:
            case RawEntryType::Semicolon:
            case RawEntryType::Colon:
            case RawEntryType::String:
            case RawEntryType::Comment:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            case RawEntryType::NotPresent:
            default:
                return false;
        }
    }
}

SequenceMatchResult VisualLinkerScript::ParsingEngine::SubParsers::MatchSequenceAnyContentWithinEnclosure(
    const CLinkerScriptFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
    const std::string& start,
    const std::vector<std::string>& enclosingContent,
    const std::string& end,
    const bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive))
    {
        return SequenceMatchResult();
    }

    matchingElements.emplace_back(*iterator);
    auto contentMatchFound = false;
    ++iterator;

    for (const auto& entry: enclosingContent)
    {
        if (iterator == endOfVectorIterator)
        {
            break;
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive) || iterator->EntryType() == RawEntryType::Comment)
        {
            ++iterator;
            continue;
        }

        matchingElements.emplace_back(*iterator);
        contentMatchFound = true;
        ++iterator;
        break;
    }

    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return {};
    }

    if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), end, !caseSensitive))
    {
        return {};
    }

    matchingElements.emplace_back(*iterator);    
    return SequenceMatchResult(contentMatchFound, matchingElements, iterator);
}


SequenceMatchResult VisualLinkerScript::ParsingEngine::SubParsers::MatchSequenceOpenEnded(
    const CLinkerScriptFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
    const std::vector<std::string>& expectedExactSequence,
    const bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return {};
    }

    matchingElements.emplace_back(*iterator);

    for (const auto& entry: expectedExactSequence)
    {
        if ((iterator == endOfVectorIterator) || (iterator->EntryType() == RawEntryType::Comment))
        {
            return {};
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive))
        {
            return {};
        }

        matchingElements.emplace_back(*iterator);
        ++iterator;
    }

    return SequenceMatchResult(true, matchingElements, iterator -1);
}

SequenceMatchResult VisualLinkerScript::ParsingEngine::SubParsers::MatchSequenceAnyContent(
    const CLinkerScriptFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
    const std::vector<std::string>& allEligibleEntries,
    const bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return {};
    }

    auto resolvedContent = linkerScriptFile.ResolveRawEntry(*iterator);
    matchingElements.emplace_back(*iterator);
    auto matchSuccess = StringIn(resolvedContent, allEligibleEntries, caseSensitive);
    return SequenceMatchResult(matchSuccess, matchingElements, iterator);
}


std::vector<CRawEntry>::const_iterator VisualLinkerScript::ParsingEngine::SubParsers::FindNextNonCommentEntry(
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator startingPoint,
		const std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    while ((startingPoint != endOfVectorIterator) && (startingPoint->EntryType() == RawEntryType::Comment))
    {
        ++startingPoint;
    }

    return startingPoint;
}


CRawEntry VisualLinkerScript::ParsingEngine::SubParsers::FuseEntriesToFormAWilcardWord(
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
		const std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{    
    if (!CanBeStartOfWildcard(linkerScriptFile, *iterator))
    {
        throw std::invalid_argument("Starting entry not a valid wildcard/word.");
    }

    auto copyOfIterator = iterator;
    const auto startLine = iterator->StartLineNumber();
    const auto startPosition = iterator->StartPosition();
    uint32_t previousEntryEndPosition;
    auto firstEntry = true;
    while (copyOfIterator != endOfVectorIterator && CanBePartOfWildcard(linkerScriptFile, *copyOfIterator))
    {
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*copyOfIterator);
        if ((copyOfIterator->StartLineNumber() != startLine) || !CanBePartOfWildcard(linkerScriptFile, *copyOfIterator))
        {            
            break;
        }

        if (!firstEntry && (copyOfIterator->StartPosition() != previousEntryEndPosition + 1)) // Spaces breaking the fusing.
        {
            break;
        }

        firstEntry = false;
        previousEntryEndPosition = copyOfIterator->StartPosition() + copyOfIterator->Length() - 1;
        ++copyOfIterator;
    }

    if (copyOfIterator == iterator) {
        if (copyOfIterator == endOfVectorIterator) {
            return CRawEntry(); // This by default creates a 'NotPresent' entry
        }
        return *copyOfIterator;
    }
    else
    {
        --copyOfIterator;
        iterator = copyOfIterator;
        auto newWordLength = copyOfIterator->Length() + copyOfIterator->StartPosition() - startPosition;
        return CRawEntry(RawEntryType::Wildcard,
                         startLine,
						 copyOfIterator->StartIndexInLine(),
						 startLine, // Not: Words are never spanned across multiple lines.
						 copyOfIterator->StartIndexInLine() + newWordLength - 1,
                         startPosition,
						 newWordLength,
                         copyOfIterator->ParenthesisDepth(),
                         copyOfIterator->ScopeDepth());
    }
}


bool VisualLinkerScript::ParsingEngine::SubParsers::AdvanceToNextNonCommentEntry(
    const CLinkerScriptFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator& iterator,
    const std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    if (iterator->EntryType() != RawEntryType::Comment)
    {
        return true;
    }

    while ((iterator != endOfVectorIterator) && (iterator->EntryType() != RawEntryType::Comment))
    {
        ++iterator;
    }

    return (iterator != endOfVectorIterator);
}
