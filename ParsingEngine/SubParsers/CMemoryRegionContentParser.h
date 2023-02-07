#ifndef CMEMORY_REGION_CONTENT_PARSER_H__
#define CMEMORY_REGION_CONTENT_PARSER_H__

#include "CSubParserBase.h"
#include "CAssignmentParser.h"
#include "SubParserType.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CMemoryStatement.h"
#include "CMemoryStatementAttributeParser.h"
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge of parsing the content found inside "MEMORY" region within a linker-script
    class CMemoryRegionContentParser : public CSubParserBase<CMemoryStatement>
    {
    private:
        CAssignmentParser m_assignmentParser;
        CMemoryStatementAttributeParser m_attributeParser;

    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::MemoryRegionContentParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CMemoryStatement> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
