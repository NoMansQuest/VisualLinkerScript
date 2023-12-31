#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "SubParserHelpers.h"
#include "../../Helpers.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../../Models/Raw/CRawFile.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

namespace
{
    bool CanBePartOfWildcard(CRawFile& linkerScriptFile, CRawEntry rawEntry)
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

    bool CanBeStartOfWildcard(CRawFile& linkerScriptFile, CRawEntry rawEntry)
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
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::string start,
    std::vector<std::string> enclosingContent,
    std::string end,
    bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive))
    {
        return SequenceMatchResult();
    }

    matchingElements.emplace_back(*iterator);
    auto contentMatchFound = false;
    iterator++;

    for (auto entry: enclosingContent)
    {
        if (iterator == endOfVectorIterator)
        {
            break;
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive) || iterator->EntryType() == RawEntryType::Comment)
        {
            iterator++;
            continue;
        }

        matchingElements.emplace_back(*iterator);
        contentMatchFound = true;
        iterator++;
        break;
    }

    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return SequenceMatchResult();
    }

    if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), end, !caseSensitive))
    {
        return SequenceMatchResult();
    }

    matchingElements.emplace_back(*iterator);    
    return SequenceMatchResult(contentMatchFound, std::move(matchingElements), iterator);
}


SequenceMatchResult VisualLinkerScript::ParsingEngine::SubParsers::MatchSequenceOpenEnded(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::vector<std::string> expectedExactSequence,
    bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return SequenceMatchResult();
    }

    matchingElements.emplace_back(*iterator);

    for (auto entry: expectedExactSequence)
    {
        if ((iterator == endOfVectorIterator) || (iterator->EntryType() == RawEntryType::Comment))
        {
            return SequenceMatchResult();
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive))
        {
            return SequenceMatchResult();
        }

        matchingElements.emplace_back(*iterator);
        iterator++;
    }

    return SequenceMatchResult(true, std::move(matchingElements), iterator -1);
}


SequenceMatchResult VisualLinkerScript::ParsingEngine::SubParsers::MatchSequenceAnyContent(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::vector<std::string> allEligibleEntries,
    bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return SequenceMatchResult();
    }

    auto resolvedContent = linkerScriptFile.ResolveRawEntry(*iterator);
    matchingElements.emplace_back(*iterator);
    auto matchSuccess = StringIn(resolvedContent, allEligibleEntries, caseSensitive);
    return SequenceMatchResult(matchSuccess, std::move(matchingElements), iterator);
}


std::vector<CRawEntry>::const_iterator VisualLinkerScript::ParsingEngine::SubParsers::FindNextNonCommentEntry(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator startingPoint,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    while ((startingPoint != endOfVectorIterator) && (startingPoint->EntryType() == RawEntryType::Comment))
    {
        startingPoint++;
    }

    return startingPoint;
}


CRawEntry VisualLinkerScript::ParsingEngine::SubParsers::FuseEntriesToFormAWilcardWord(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{    
    if (!CanBeStartOfWildcard(linkerScriptFile, *iterator))
    {
        throw std::invalid_argument("Starting entry not a valid wildcard/word.");
    }

    std::vector<CRawEntry>::const_iterator copyOfIterator = iterator;
    auto startLine = iterator->StartLineNumber();
    auto startPosition = iterator->StartPosition();
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
        copyOfIterator++;
    }

    if (copyOfIterator == iterator) {
        if (copyOfIterator == endOfVectorIterator) {
            return CRawEntry(); // This by default creates a 'NotPresent' entry
        }
        return *copyOfIterator;
    }
    else
    {
        copyOfIterator--;
        iterator = copyOfIterator;
        return CRawEntry(RawEntryType::Wildcard,
                         startLine,
                         startPosition,
                         copyOfIterator->Length() + copyOfIterator->StartPosition() - startPosition,
                         copyOfIterator->ParenthesisDepth(),
                         copyOfIterator->ScopeDepth());
    }
}


bool VisualLinkerScript::ParsingEngine::SubParsers::AdvanceToNextNonCommentEntry(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator& iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    if (iterator->EntryType() != RawEntryType::Comment)
    {
        return true;
    }

    while ((iterator != endOfVectorIterator) && (iterator->EntryType() != RawEntryType::Comment))
    {
        iterator++;
    }

    return (iterator != endOfVectorIterator);
}
