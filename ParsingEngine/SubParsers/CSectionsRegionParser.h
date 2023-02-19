#ifndef CSECTIONS_REGION_PARSER_H__
#define CSECTIONS_REGION_PARSER_H__

#include "SubParserType.h"
#include "CSectionOutputStatementParser.h"
#include "CScopedRegionParser.h"
#include "../Models/CSectionsRegion.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "SECTIONS" region inside a linker-script
    class CSectionsRegionParser : public CScopedRegionParser<SubParserType::SectionsRegionParser, CSectionOutputStatementParser, CSectionsRegion>
    {
    protected:
        /// @copydoc CScopedRegionParser:GetHeaderName()
        virtual std::string GetHeaderName() override
        {
            return "SECTIONS";
        }
    };
}

#endif
