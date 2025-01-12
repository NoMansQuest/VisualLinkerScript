#ifndef CINPUTSECTIONFUNCTIONSORTPARSER_H
#define CINPUTSECTIONFUNCTIONSORTPARSER_H

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CFunctionCall.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge parsing a full function: <NAME>(<EXPRESSION>)
    class CInputSectionFunctionSortParser : public CSubParserBase<CFunctionCall>
    {
    public:

        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::InputSectionFunctionSortParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CFunctionCall> TryParse(
				const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif // CINPUTSECTIONFUNCTIONSORTPARSER_H
