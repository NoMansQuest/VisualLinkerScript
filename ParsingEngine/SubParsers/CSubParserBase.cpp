#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CSubParserBase.h"
#include "../../Helpers.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../../Models/Raw/CRawFile.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;


SequenceMatchResult MatchSequenceAnyContentWithinEnclosure(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::string start,
    std::vector<std::string> enclosingContent,
    std::string end,
    bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator) ||
        !StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive))
    {
        return SequenceMatchResult();
    }

    matchingElements.emplace_back(*iterator);

    for (auto entry: enclosingContent)
    {
        if ((++iterator == endOfVectorIterator) || (iterator->EntryType() == RawEntryType::Comment))
        {
            return SequenceMatchResult();
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive))
        {
            return SequenceMatchResult();
        }

        matchingElements.emplace_back(*iterator);
    }

    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return SequenceMatchResult();
    }

    matchingElements.emplace_back(*iterator);

    auto matchSuccess = StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive);
    return SequenceMatchResult(matchSuccess, std::move(matchingElements), iterator);
}


SequenceMatchResult MatchSequenceOpenEnded(
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

    return SequenceMatchResult(true, std::move(matchingElements), iterator - 1);
}


SequenceMatchResult MatchSequenceAnyContent(
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


std::vector<CRawEntry>::const_iterator FindNextNonCommentEntry(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator startingPoint,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    while ((startingPoint != endOfVectorIterator) && (startingPoint->EntryType() != RawEntryType::Comment))
    {
        startingPoint++;
    }

    return startingPoint;
}


bool AdvanceToNextNonCommentEntry(
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
