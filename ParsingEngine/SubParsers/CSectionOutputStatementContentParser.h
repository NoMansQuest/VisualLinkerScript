#ifndef CSECTION_OUTPUT_STATEMENT_CONTENT_PARSER_H__
#define CSECTION_OUTPUT_STATEMENT_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/CSectionOutputStatement.h"
#include "../Models/Raw/CRawFile.h"
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the content of the "SECTIONS" region inside a linker-script
    class CSectionOutputStatementContentParser : public CSubParserBase<CSectionOutputStatement>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::SectionsOutputStatementContentParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CSectionOutputStatement> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif // CSECTION_OUTPUT_STATEMENT_CONTENT_PARSER_H__
