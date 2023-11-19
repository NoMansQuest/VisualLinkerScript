#ifndef SUBPARSERHELPERS_H
#define SUBPARSERHELPERS_H

#include <vector>
#include <memory>
#include <type_traits>

#include "SubParserType.h"
#include "../../Models/CLinkerScriptContentBase.h"
#include "../../Models/Raw/CRawFile.h"


namespace VisualLinkerScript::ParsingEngine::SubParsers
{

    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Object used by all MatchSequenceXXXX functions.
    /// @return
    class SequenceMatchResult
    {
        public:
            /// @brief Instantiates a new instance of SequenceMatchResulted
            SequenceMatchResult(bool successful, std::vector<CRawEntry>&& matchedElements)
                : m_matchedElements(matchedElements),
                  m_successful(successful)
            {}

            /// @brief Instantiates a new instance of SequenceMatchResulted
            SequenceMatchResult()
                : m_matchedElements(),
                  m_successful(false)
            {}

        private:
            std::vector<CRawEntry> m_matchedElements;
            bool m_successful;

        public:
            /// @brief Reports back whether the match was successful
            bool Successful() { return m_successful; }

            /// @brief Reports backs a list of matched elements, in the order of matching
            const std::vector<CRawEntry>& MatchedElements() { return m_matchedElements; }
    };

    /// @brief Checks if the <start> [any entry in 'enclosingContent'] <stop> is present.
    SequenceMatchResult MatchSequenceAnyContentWithinEnclosure(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator,
            std::string start,
            std::vector<std::string> enclosingContent,
            std::string end,
            bool caseSensitive = true);

    /// @brief Checks if the exact sequence defiend in 'expectedExactSequence' is present.
    SequenceMatchResult MatchSequenceOpenEnded(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator,
            std::vector<std::string> expectedExactSequence,
            bool caseSensitive = true);

    /// @brief Checks if any of the eligibile entries are present right at iterator start.
    SequenceMatchResult MatchSequenceAnyContent(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator,
            std::vector<std::string> allEligibleEntries,
            bool caseSensitive = true);

    /// @brief Advances current iterator until either end of iterator
    ///        is reached or a non-comment entry is found.
    bool AdvanceToNextNonCommentEntry(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator& iterator,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator);

    /// @brief Finds the next non-comment iterator. Returns endOfVector if not found.
    std::vector<CRawEntry>::const_iterator FindNextNonCommentEntry(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator startingPoint,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator);

}

#endif // SUBPARSERHELPERS_H
