#ifndef CPHDRS_REGION_CONTENT_PARSER_H__
#define CPHDRS_REGION_CONTENT_PARSER_H__

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/Raw/CRawFile.h"
#include "../../Models/CPhdrsStatement.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CPhdrsRegionContentParser : public CSubParserBase<CPhdrsStatement>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::PhdrsRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CPhdrsStatement> TryParse(
				const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
