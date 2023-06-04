#ifndef CVERSION_REGION_PARSER_H__
#define CVERSION_REGION_PARSER_H__

#include <memory>

#include "CScopedRegionParser.h"
#include "CVersionRegionContentParser.h"
#include "SubParserType.h"

#include "../../Models/CVersionsRegion.h"


namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "VERSIONS" region inside a linker-script
    class CVersionRegionParser : public CScopedRegionParser<SubParserType::VersionRegionParser, CVersionRegionContentParser, CVersionsRegion>
    {
    protected:
        /// @copydoc CScopedRegionParser:GetHeaderName()
        virtual std::string GetHeaderName() override
        {
            return "VERSION";
        }
    };
}

#endif
