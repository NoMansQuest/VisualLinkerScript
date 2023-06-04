#ifndef CSECTIONS_REGION_PARSER_H__
#define CSECTIONS_REGION_PARSER_H__

#include "SubParserType.h"
#include "CSubParserBase.h"

#include "../../Models/CSectionsRegion.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "SECTIONS" region inside a linker-script
    class CSectionsRegionParser: public CSubParserBase<CSectionsRegion>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::SectionsRegionParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CSectionsRegion> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
