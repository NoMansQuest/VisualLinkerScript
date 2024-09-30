#include <vector>
#include <string>

#include "CSubParserBase.h"
#include "SubParserHelpers.h"
#include "../../Helpers.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

SequenceMatchResult MatchSequenceAnyContentWithinEnclosure(
    const CLinkerScriptFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
    const std::string& start,
    const std::vector<std::string>& enclosingContent,
    const std::string& end,
    const bool caseSensitive)
{
    std::vector<CRawEntry> matchingElements;
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator) ||
        !StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive))
    {
        return {};
    }

    matchingElements.emplace_back(*iterator);

    for (const auto& entry: enclosingContent)
    {
        if ((++iterator == endOfVectorIterator) || (iterator->EntryType() == RawEntryType::Comment))
        {
            return {};
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive))
        {
            return {};
        }

        matchingElements.emplace_back(*iterator);
    }

    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return {};
    }

    matchingElements.emplace_back(*iterator);

    const auto matchSuccess = StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive);
    return SequenceMatchResult(matchSuccess, matchingElements, iterator);
}

SequenceMatchResult MatchSequenceOpenEnded(
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

    return SequenceMatchResult(true, matchingElements, iterator - 1);
}

SequenceMatchResult MatchSequenceAnyContent(
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

    const auto resolvedContent = linkerScriptFile.ResolveRawEntry(*iterator);
    matchingElements.emplace_back(*iterator);
    const auto matchSuccess = StringIn(resolvedContent, allEligibleEntries, caseSensitive);
    return SequenceMatchResult(matchSuccess, matchingElements, iterator);
}

std::vector<CRawEntry>::const_iterator FindNextNonCommentEntry(
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator startingPoint,
		const std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    while ((startingPoint != endOfVectorIterator) && (startingPoint->EntryType() != RawEntryType::Comment))
    {
        ++startingPoint;
    }

    return startingPoint;
}


bool AdvanceToNextNonCommentEntry(
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
