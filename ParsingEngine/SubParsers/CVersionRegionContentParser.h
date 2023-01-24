#ifndef CVERSIONS_REGION_CONTENT_PARSER_H__
#define CVERSIONS_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CVersionScope.h"
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the content found inside "VERSION" region within a linker-script
    class CVersionRegionContentParser : public CSubParserBase<CVersionScope>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::VersionRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CVersionScope> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
