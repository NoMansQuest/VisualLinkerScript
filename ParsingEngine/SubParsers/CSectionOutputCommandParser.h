#ifndef CSECTIONS_REGION_CONTENT_PARSER_H__
#define CSECTIONS_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../../Models/CSectionOutputCommand.h"
#include "../../Models/Raw/CRawFile.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Object in charge of parsing the content of the "SECTIONS" region inside a linker-script
    class CSectionOutputCommandParser : public CSubParserBase<CSectionOutputCommand>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::SectionsOutputCommandParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CSectionOutputCommand> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
