#ifndef CDEFAULT_PARSER_H__
#define CDEFAULT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge parsing R-Value expression (e.g. a + b - c + ((f / g) - h) ...)
    class CExpressionParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::ExpressionParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& TryParse(
                std::vector<CRawEntry>::const_iterator& iterator) override;
    }
}

#endif