#ifndef CINPUTSECTIONFUNCTIONKEEPPARSER_H
#define CINPUTSECTIONFUNCTIONKEEPPARSER_H

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CFunctionCall.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge parsing 'KEEP' function calls: KEEP(<SeciontOutputCommand>.
    /// @example KEEP (*(.eh_frame))
    class CInputSectionFunctionKeepParser : public CSubParserBase<CFunctionCall>
    {
    public:

        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return SubParserType::InputSectionFunctionKeepParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CFunctionCall> TryParse(
                const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}


#endif // CINPUTSECTIONFUNCTIONKEEPPARSER_H
