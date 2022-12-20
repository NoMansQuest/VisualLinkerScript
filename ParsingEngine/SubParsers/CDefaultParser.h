#ifndef CDEFAULT_PARSER_H__
#define CDEFAULT_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing content that is unrecognizable or irrelevant (such as comments)
    class CDefaultParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual bool TryParse(std::vector<CRawEntry>::const_iterator& iterator) override;
    }
}

#endif