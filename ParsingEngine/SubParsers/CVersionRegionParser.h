#ifndef CVERSION_REGION_PARSER_H__
#define CVERSION_REGION_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "VERSIONS" region inside a linker-script
    class CVersionRegionParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<CLinkerScriptContentBase>&& TryParse(std::vector<CRawEntry>::const_iterator& iterator) override;    
    }
}

#endif