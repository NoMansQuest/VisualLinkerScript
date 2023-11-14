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


bool MatchSequenceAnyContentWithinEnclosure(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::string start,
    std::vector<std::string> enclosingContent,
    std::string end,
    bool caseSensitive)
{
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator) ||
        !StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive))
    {
        return false;
    }

    for (auto entry: enclosingContent)
    {
        if ((++iterator == endOfVectorIterator) || (iterator->EntryType() == RawEntryType::Comment))
        {
            return false;
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive))
        {
            return false;
        }
    }

    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return false;
    }

    return StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), start, !caseSensitive);
}


bool MatchSequenceOpenEnded(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::vector<std::string> expectedExactSequence,
    bool caseSensitive)
{
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return false;
    }

    for (auto entry: expectedExactSequence)
    {
        if ((iterator == endOfVectorIterator) || (iterator->EntryType() == RawEntryType::Comment))
        {
            return false;
        }

        if (!StringEquals(linkerScriptFile.ResolveRawEntry(*iterator), entry, !caseSensitive))
        {
            return false;
        }

        iterator++;
    }

    return true;
}


bool MatchSequenceAnyContent(
    CRawFile& linkerScriptFile,
    std::vector<CRawEntry>::const_iterator iterator,
    std::vector<CRawEntry>::const_iterator endOfVectorIterator,
    std::vector<std::string> allEligibleEntries,
    bool caseSensitive)
{
    if (!AdvanceToNextNonCommentEntry(linkerScriptFile, iterator, endOfVectorIterator))
    {
        return false;
    }

    auto resolvedContent = linkerScriptFile.ResolveRawEntry(*iterator);
    return (StringIn(resolvedContent, allEligibleEntries, caseSensitive));
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
