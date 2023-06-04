#ifndef CINPUT_SECTION_FUNCTION_PARSER_H__
#define CINPUT_SECTION_FUNCTION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../../Models/CInputSectionFunction.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge parsing a full function: <NAME>(<EXPRESSION>)
    class CInputSectionFunctionParser : public CSubParserBase<CInputSectionFunction>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::InputSectionFunctionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CInputSectionFunction> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
