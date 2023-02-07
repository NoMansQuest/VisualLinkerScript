#ifndef CSECTION_OUTPUT_TYPE_PARSER_H__
#define CSECTION_OUTPUT_TYPE_PARSER_H__

#include <memory>
#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CSectionOutputType.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CSectionOutputTypeParser : public CSubParserBase<CSectionOutputType>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::MemoryStatementAttributeParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CSectionOutputType> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif // CSECTIONOUTPUTTYPEPARSER_H
