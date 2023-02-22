#ifndef CINPUT_SECTION_PARSER_H__
#define CINPUT_SECTION_PARSER_H__

#include <memory>
#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/CInputSection.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge parsing a Input-Section: Filter(files)
    ///        Example: *[A-Z](.text)  OR  *(.text .bss*)
    class CInputSectionParser : public CSubParserBase<CInputSection>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::InputSectionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CInputSection> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
