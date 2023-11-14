#ifndef CSECTION_OUTPUT_OVERLAYS_PARSER_H__
#define CSECTION_OUTPUT_OVERLAYS_PARSER_H__

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CSectionOutputCommand.h"
#include "../../Models/Raw/CRawFile.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing the "OVERLAY" section command
    class CSectionOutputOverlayParser : public CSubParserBase<CSectionOutputCommand>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::SectionsOutputOverlayParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CSectionOutputCommand> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
