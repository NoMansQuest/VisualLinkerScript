#ifndef CMEMORY_REGION_PARSER_H__
#define CMEMORY_REGION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "MEMORY" region inside a linker-script
    class CMemoryParserRegion : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::MemoryRegionParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& TryParse(
                std::vector<CRawEntry>::const_iterator& iterator) override;
    }
}

#endif