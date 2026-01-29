#ifndef CSUBPARSER_BASE_H__
#define CSUBPARSER_BASE_H__

#include <vector>
#include <memory>
#include <type_traits>

#include "ParsingEngine/SubParsers/SubParserType.h"

#include "Models/CParsedContentBase.h"
#include "Models/CLinkerScriptFile.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    #define UNREFERENCED_PARAMETER(P) (P)

    using namespace VisualLinkerScript::Models::Raw;
    using namespace VisualLinkerScript::Models;

    /// @brief Base class for all sub-parsers
    template<typename TOutputType>
    class CSubParserBase
    {
        static_assert(std::is_base_of<CParsedContentBase, TOutputType>::value);

    public:
        /// @brief Type of parser.
        virtual SubParserType Type() = 0;

        /// @brief Try to parse the content. If successful, parsed content can be recovered via @see {GetParsedContent}.
        /// @param linkerScriptFile The linker script file
        /// @param iterator Iterator pointing out to the location to start parsing
        /// @param endOfVectorIterator Iterator value pointing to the end of the vector (hence end of parsing)
        /// @return Parsed content (if successful), under which case the @see {iterator} is also updated, otherwise an empty vector in case
        ///         of failure to parse
        virtual std::shared_ptr<TOutputType> TryParse(
                const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) = 0;
    };
}

#endif
