#ifndef CSYMBOLASSIGNMENT_PARSER_H__
#define CSYMBOLASSIGNMENT_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing symbol assignment (i.e. a = b + c...) inside a linker-script
    class CSymbolAssignmentParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<CLinkerScriptContentBase>&& TryParse(std::vector<CRawEntry>::const_iterator& iterator) override;    
    }
}

#endif