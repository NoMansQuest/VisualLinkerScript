#ifndef CMEMORY_STATEMENT_ATTRIBUTE_PARSER_H__
#define CMEMORY_STATEMENT_ATTRIBUTE_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CMemoryStatementAttributeParser : public CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::MemoryStatementAttributeParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
