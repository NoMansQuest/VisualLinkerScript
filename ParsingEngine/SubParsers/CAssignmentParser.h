#ifndef CASSIGNMENT_PARSER_H__
#define CASSIGNMENT_PARSER_H__

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CAssignmentStatement.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Object in charge of parsing symbol assignment (i.e. a = b + c...) inside a linker-script
    class CAssignmentParser : public CSubParserBase<CAssignmentStatement>
    {
    private:
        bool m_isInsideProcedure;

    public:
        /// @brief Default constructor.
        CAssignmentParser(bool isInsideProcedure = false) :
            m_isInsideProcedure(isInsideProcedure)
        {}

        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::AssignmentParser;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CAssignmentStatement> TryParse(
                const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
