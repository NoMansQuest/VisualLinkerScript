#ifndef CSECTIONS_REGION_OVERLAY_PARSER_H__
#define CSECTIONS_REGION_OVERLAY_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{    
    /// @brief Object in charge of parsing the "OVERLAY" entries found under "SECTIONS" region inside a linker-script
    class CSectionsRegionOverlayParsers : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::SectionsRegionOverlayParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& TryParse(
                std::vector<CRawEntry>::const_iterator& iterator) override;    
    }
}

#endif