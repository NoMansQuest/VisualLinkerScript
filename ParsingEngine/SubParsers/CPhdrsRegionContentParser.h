#ifndef CPHDRS_REGION_CONTENT_PARSER_H__
#define CPHDRS_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CPhdrsRegionContentParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::PhdrsRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& CPhdrsRegionParser::TryParse(
                    std::string& fileContent,
                    std::vector<CRawEntry>::const_iterator& iterator,
                    std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;      
    }
}

#endif