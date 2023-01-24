#ifndef CPHDRS_REGION_CONTENT_PARSER_H__
#define CPHDRS_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "CExpressionParser.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include <memory>
#include "../Models/CPhdrsStatement.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CPhdrsRegionContentParser : public CSubParserBase<CPhdrsStatement>
    {
    private:
        CExpressionParser m_expressionParser;

    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::PhdrsRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CPhdrsStatement> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
