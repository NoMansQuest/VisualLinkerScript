#ifndef CSUBPARSER_BASE_H__
#define CSUBPARSER_BASE_H__

#include <vector>
#include "../Models/CLinkerScriptContentBase.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;    
using namespace VisualLinkerScript::ParsingEngine::Models;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Base class for all sub-parsers
    class CSubParserBase
    {
    public:
        /// @brief Try to parse the content. If succesful, parsed content can be recovered via @see {GetParsedContent}.
        /// @param iterator [In] Iterator pointing out to the location to start parsing
        /// @return Parsed content (if successful), under which case the @see {iterator} is also updated, otherwise an empty vectoro in case
        ///         of failure to parse
        virtual std::vector<CLinkerScriptContentBase>&& TryParse(std::vector<CRawEntry>::const_iterator& iterator) = 0;
    }
}

#endif