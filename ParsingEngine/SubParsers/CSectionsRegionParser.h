#ifndef CSECTIONS_REGION_PARSER_H__
#define CSECTIONS_REGION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{    
    /// @brief Object in charge of parsing the "SECTIONS" region inside a linker-script
    class CSectionsRegionParsers : CSubParserBase
    {
    public:
        /// @brief Reports back the type of subparser. This is to avoid "typeid" and "dynamic_casts" later on.        
        virtual SubParserType Type() = 0;

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& TryParse(
                std::vector<CRawEntry>::const_iterator& iterator) override;    
    }
}

#endif