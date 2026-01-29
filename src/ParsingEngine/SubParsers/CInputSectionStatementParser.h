#ifndef CINPUT_SECTION_PARSER_H__
#define CINPUT_SECTION_PARSER_H__

#include <memory>

#include "ParsingEngine/SubParsers/CSubParserBase.h"
#include "ParsingEngine/SubParsers/SubParserType.h"

#include "Models/CInputSectionStatement.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge parsing a Input-Section: Filter(files)
    ///        Example: *[A-Z](.text)  OR  *(.text .bss*)
    class CInputSectionStatementParser : public CSubParserBase<CInputSectionStatement>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::InputSectionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CInputSectionStatement> TryParse(
				const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
