#ifndef CSECTIONS_REGION_PARSER_H__
#define CSECTIONS_REGION_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{    
    /// @brief Object in charge of parsing the "SECTIONS" region inside a linker-script
    class CSectionsRegionParsers : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<CLinkerScriptContentBase>&& TryParse(std::vector<CRawEntry>::const_iterator& iterator) override;    
    }
}

#endif