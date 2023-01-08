#ifndef CPHDRS_REGION_PARSER_H__
#define CPHDRS_REGION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include "CPhdrsRegionContentParser.h"
#include "../Models/Raw/CRawFile.h"
#include <memory>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CPhdrsRegionParser : public CSubParserBase
    {
    private:
        CPhdrsRegionContentParser m_phdrsRegionContentParser;

    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::PhdrsRegionParser;
        }

        /// @copydoc CSubParserBase::TryParse
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
