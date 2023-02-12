#ifndef CSECTIONS_REGION_CONTENT_PARSER_H__
#define CSECTIONS_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "CExpressionParser.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the content of the "SECTIONS" region inside a linker-script
    class CSectionsRegionContentParser : public CSubParserBase<CLinkerScriptContentBase>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::SectionsRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif // CSECTIONSREGIONCONTENTPARSER_H
