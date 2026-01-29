#ifndef CMEMORY_REGION_PARSER_H__
#define CMEMORY_REGION_PARSER_H__

#include "ParsingEngine/SubParsers/CMemoryRegionContentParser.h"
#include "ParsingEngine/SubParsers/CScopedRegionParser.h"
#include "ParsingEngine/SubParsers/SubParserType.h"
#include "Models/CMemoryRegion.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "MEMORY" region inside a linker-script
    class CMemoryParserRegion : public CScopedRegionParser<SubParserType::MemoryRegionParser, CMemoryRegionContentParser, CMemoryRegion>
    {
    protected:
        /// @copydoc CScopedRegionParser:GetHeaderName()
        virtual std::string GetHeaderName() override
        {
            return "MEMORY";
        }
    };
}

#endif
