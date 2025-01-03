#ifndef CMEMORY_REGION_CONTENT_PARSER_H__
#define CMEMORY_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../../Models/Raw/CRawFile.h"
#include "../../Models/CMemoryStatement.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the content found inside "MEMORY" region within a linker-script
    class CMemoryRegionContentParser : public CSubParserBase<CMemoryStatement>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::MemoryRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CMemoryStatement> TryParse(
				const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
