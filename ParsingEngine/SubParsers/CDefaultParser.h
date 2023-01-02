#ifndef CDEFAULT_PARSER_H__
#define CDEFAULT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing content that is unrecognizable or irrelevant (such as comments)
    class CDefaultParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::DefaultParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& TryParse(
                std::vector<CRawEntry>::const_iterator& iterator) override;
    }
}

#endif