#ifndef CSUBPARSER_BASE_H__
#define CSUBPARSER_BASE_H__

#include <vector>
#include <memory>
#include <type_traits>

#include "SubParserType.h"

#include "../../Models/CLinkerScriptContentBase.h"
#include "../../Models/Raw/CRawFile.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    #define UNREFERENCED_PARAMETER(P) (P)

    using namespace VisualLinkerScript::Models::Raw;
    using namespace VisualLinkerScript::Models;

    /// @brief Object used by all MatchSequenceXXXX functions.
    /// @return 
    struct SequenceMatchResult
    {
        public: 
            /// @brief Instantiates a new instance of SequenceMatchResulted    
            SequenceMatchResult(bool successful, std::vector<CRawEntry>&& matchedElements)
                : m_successful(successful),
                  m_matchedElements(matchedElements)
            {}      

        private:
            std::vector<CRawEntry> m_matchedElements;
            bool m_successful;        

        public: 
            /// @brief Reports back whether the match was successful       
            bool Successful { return m_successful; } 

            /// @brief Reports backs a list of matched elements, in the order of matching
            std::vector<CRawEntry>& MatchedElements() const { return m_matchedElements; }                
    };

    /// @brief Checks if the <start> [any entry in 'enclosingContent'] <stop> is present.
    bool MatchSequenceAnyContentWithinEnclosure(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator,
            std::string start,
            std::vector<std::string> enclosingContent,
            std::string end,
            bool caseSensitive = true);

    /// @brief Checks if the exact sequence defiend in 'expectedExactSequence' is present.
    bool MatchSequenceOpenEnded(
            CRawFile& linkerScriptFile,
            std::vector<CRawEntry>::const_iterator iterator,
            std::vector<CRawEntry>::const_iterator endOfVectorIterator,
            std::vector<std::string> expectedExactSequence,
            bool caseSensitive = true);

    /// @brief Checks if any of the eligibile entries are present right at iterator start.
    bool MatchSequenceAnyContent(
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


    /// @brief Base class for all sub-parsers
    template<typename TOutputType>
    class CSubParserBase
    {
        static_assert(std::is_base_of<CLinkerScriptContentBase, TOutputType>::value);

    public:
        /// @brief Type of parser.
        virtual SubParserType Type() = 0;

        /// @brief Try to parse the content. If succesful, parsed content can be recovered via @see {GetParsedContent}.
        /// @param fileContent Content of the linker-script file
        /// @param iterator Iterator pointing out to the location to start parsing
        /// @param endOfVectorIterator Iterator value pointing to the end of the vector (hence end of parsing)
        /// @return Parsed content (if successful), under which case the @see {iterator} is also updated, otherwise an empty vectoro in case
        ///         of failure to parse
        virtual std::shared_ptr<TOutputType> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) = 0;
    };
}

#endif
