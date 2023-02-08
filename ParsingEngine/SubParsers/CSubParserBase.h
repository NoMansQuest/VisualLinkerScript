#ifndef CSUBPARSER_BASE_H__
#define CSUBPARSER_BASE_H__

#include <vector>
#include <memory>
#include "../Models/CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawFile.h"
#include "SubParserType.h"
#include <type_traits>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;    
using namespace VisualLinkerScript::ParsingEngine::Models;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
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
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) = 0;
    };
}

#endif
