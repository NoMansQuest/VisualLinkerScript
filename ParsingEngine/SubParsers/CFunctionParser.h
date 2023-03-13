#ifndef CFUNCTION_PARSER_H__
#define CFUNCTION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "../Models/CFunctionCall.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge parsing a full function: <NAME>(<EXPRESSION>)
    class CFunctionParser : public CSubParserBase<CFunctionCall>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::FunctionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CFunctionCall> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}


#endif
