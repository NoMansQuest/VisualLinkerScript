#ifndef CFUNCTION_PARSER_H__
#define CFUNCTION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "CExpressionParser.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge parsing a full function: <NAME>(<EXPRESSION>)
    class CFunctionParser : public CSubParserBase
    {
    private:
        CExpressionParser m_expressionParser;

    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::FunctionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}


#endif