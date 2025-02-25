#ifndef CVERSION_REGION_CONTENT_PARSER_H__
#define CVERSION_REGION_CONTENT_PARSER_H__

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CVersionScope.h"
#include "../../Models/Raw/CRawFile.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Object in charge of parsing the content of the "VERSION" region inside a linker-script
    class CVersionRegionContentParser : public CSubParserBase<CVersionScope>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::VersionRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CVersionScope> TryParse(
				const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
