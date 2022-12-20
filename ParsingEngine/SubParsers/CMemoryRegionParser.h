#ifndef CMEMORY_REGION_PARSER_H__
#define CMEMORY_REGION_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "MEMORY" region inside a linker-script
    class CMemoryParserRegion : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<CLinkerScriptContentBase>&& TryParse(std::vector<CRawEntry>::const_iterator& iterator) override;
    }
}

#endif