#ifndef CASSIGNMENT_PARSER_H__
#define CASSIGNMENT_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>
#include "../Models/CAssignmentProcedureStatement.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing procedural symbol assignment (i.e. PROVIDE(a = b + c);...) inside a linker-script
    class CAssignmentProcedureParser : public CSubParserBase<CAssignmentStatement>
    {
    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::AssignmentProcedureParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CAssignmentProcedureStatement> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif