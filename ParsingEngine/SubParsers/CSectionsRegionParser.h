#ifndef CSECTIONS_REGION_PARSER_H__
#define CSECTIONS_REGION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{    
    /// @brief Object in charge of parsing the "SECTIONS" region inside a linker-script
    class CSectionsRegionParser : public CSubParserBase
    {
    public:
        /// @brief Reports back the type of subparser. This is to avoid "typeid" and "dynamic_casts" later on.        
        virtual SubParserType Type() override
        {
            return SubParserType::SectionsRegionParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
