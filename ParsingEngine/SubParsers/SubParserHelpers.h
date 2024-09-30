#ifndef SUBPARSERHELPERS_H
#define SUBPARSERHELPERS_H

#include <vector>
#include <memory>
#include <type_traits>

#include "SubParserType.h"
#include "../../Models/CParsedContentBase.h"
#include "../../Models/CLinkerScriptFile.h"


namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    using namespace VisualLinkerScript::Models;
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Object used by all MatchSequenceXXXX functions.
    /// @return
    class SequenceMatchResult
    {
        public:
            /// @brief Instantiates a new instance of SequenceMatchResulted
            SequenceMatchResult(bool successful,
                                const std::vector<CRawEntry>& matchedElements,
                                const std::vector<CRawEntry>::const_iterator& iteratorToLastElement)
                : m_matchedElements(matchedElements),
                  m_iteratorToLastElement(iteratorToLastElement),
                  m_successful(successful)
            {}

            /// @brief Instantiates a new instance of SequenceMatchResulted
            SequenceMatchResult()
                : m_matchedElements(),
                  m_iteratorToLastElement(),
                  m_successful(false)
            {}

        private:
            std::vector<CRawEntry> m_matchedElements;
            std::vector<CRawEntry>::const_iterator m_iteratorToLastElement;
            bool m_successful;

        public:
            /// @brief Reports back whether the match was successful
            bool Successful() const { return m_successful; }

            /// @brief Returns an iterator pointing to the last element matched.
            std::vector<CRawEntry>::const_iterator IteratorToLastElement() { return m_iteratorToLastElement; }

            /// @brief Reports backs a list of matched elements, in the order of matching
            const std::vector<CRawEntry>& MatchedElements() { return m_matchedElements; }
    };

    /// @brief Checks if the <start> [any entry in 'enclosingContent'] <stop> is present.
    SequenceMatchResult MatchSequenceAnyContentWithinEnclosure(
			const CLinkerScriptFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
			const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
			const std::string& start,
			const std::vector<std::string>& enclosingContent,
			const std::string& end,
            bool caseSensitive = true);

    /// @brief Checks if the exact sequence defiend in 'expectedExactSequence' is present.
    SequenceMatchResult MatchSequenceOpenEnded(
			const CLinkerScriptFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
			const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
			const std::vector<std::string>& expectedExactSequence,
            bool caseSensitive = true);

    /// @brief Checks if any of the eligibile entries are present right at iterator start.
    SequenceMatchResult MatchSequenceAnyContent(
			const CLinkerScriptFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
			const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
			const std::vector<std::string>& allEligibleEntries,
            bool caseSensitive = true);

    /// @brief Advances current iterator until either end of iterator
    ///        is reached or a non-comment entry is found.
    bool AdvanceToNextNonCommentEntry(
			const CLinkerScriptFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator& iterator,
			const std::vector<CRawEntry>::const_iterator& endOfVectorIterator);

    /// @brief Fuses words, operators and wildcars which may be forming
    ///        a 'word' under InputSection scope. This operation may
    CRawEntry FuseEntriesToFormAWilcardWord(
			const CLinkerScriptFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator& iterator,
			const std::vector<CRawEntry>::const_iterator& endOfVectorIterator);

    /// @brief Finds the next non-comment iterator. Returns endOfVector if not found.
    std::vector<CRawEntry>::const_iterator FindNextNonCommentEntry(
			const CLinkerScriptFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator startingPoint,
			const std::vector<CRawEntry>::const_iterator& endOfVectorIterator);

}

#endif // SUBPARSERHELPERS_H
