#ifndef CINPUTSECTIONFUNCTIONEXCLUDEFILEPARSER_H
#define CINPUTSECTIONFUNCTIONEXCLUDEFILEPARSER_H

#include <memory>

#include "ParsingEngine/SubParsers/CSubParserBase.h"
#include "ParsingEngine/SubParsers/SubParserType.h"

#include "Models/CFunctionCall.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge parsing the "EXCLUDE_LIST(....)" calls.
    class CInputSectionFunctionExcludeFileParser : public CSubParserBase<CFunctionCall>
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

#endif // CINPUTSECTIONFUNCTIONEXCLUDEFILEPARSER_H
