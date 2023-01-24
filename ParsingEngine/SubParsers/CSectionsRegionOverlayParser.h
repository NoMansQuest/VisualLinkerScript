#ifndef CSECTIONS_REGION_OVERLAY_PARSER_H__
#define CSECTIONS_REGION_OVERLAY_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/CSectionOverlayStatement.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{    
    /// @brief Object in charge of parsing the "OVERLAY" entries found under "SECTIONS" region inside a linker-script
    class CSectionsRegionOverlayParser : public CSubParserBase<CSectionOverlayStatement>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::SectionsRegionOverlayParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CSectionOverlayStatement> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
