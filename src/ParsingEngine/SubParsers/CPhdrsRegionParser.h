#ifndef CPHDRS_REGION_PARSER_H__
#define CPHDRS_REGION_PARSER_H__

#include "ParsingEngine/SubParsers/SubParserType.h"
#include "ParsingEngine/SubParsers/CPhdrsRegionContentParser.h"
#include "ParsingEngine/SubParsers/CScopedRegionParser.h"
#include "Models/CPhdrsRegion.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CPhdrsRegionParser : public CScopedRegionParser<SubParserType::PhdrsRegionParser, CPhdrsRegionContentParser, CPhdrsRegion>
    {
    protected:
        /// @copydoc CScopedRegionParser:GetHeaderName()
        virtual std::string GetHeaderName() override
        {
            return "PHDRS";
        }
    };
}

#endif
