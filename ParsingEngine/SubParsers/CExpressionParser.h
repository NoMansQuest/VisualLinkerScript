#ifndef CEXPRESSION_PARSER_H__
#define CEXPRESSION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>
#include "../Models/CExpression.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    enum class ExpressionTerminationType
    {
        SemicolonOrColonAsDelimiter,
        NewLineAsDelimiter
    };

    /// @brief Object in charge parsing R-Value expression (e.g. a + b - c + ((f / g) - h) ...)
    class CExpressionParser : public CSubParserBase<CExpression>
    {
    private:
        ExpressionTerminationType m_terminationType;
        bool m_supportsMultiLine;

    public:
        /// @brief Initializes an instance of 'CExpressionParser'
        CExpressionParser(ExpressionTerminationType terminationType, bool supportsMultiline)
            : m_terminationType(terminationType),
              m_supportsMultiLine(supportsMultiline)
        {}

        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::ExpressionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CExpression> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#endif
