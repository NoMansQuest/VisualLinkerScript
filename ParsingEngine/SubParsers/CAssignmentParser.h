#ifndef CASSIGNMENT_PARSER_H__
#define CASSIGNMENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing symbol assignment (i.e. a = b + c...) inside a linker-script
    class CAssignmentParser : public CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::AssignmentParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
