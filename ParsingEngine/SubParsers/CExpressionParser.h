#ifndef CEXPRESSION_PARSER_H__
#define CEXPRESSION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>
#include "../Models/CExpression.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    enum class ExpressionParserType
    {
        NormalParser,
        TernaryThenSectionParser,
        TernaryElseSectionParser
    };

    /// @brief Object in charge parsing R-Value expression (e.g. a + b - c + ((f / g) - h) ...)
    class CExpressionParser : public CSubParserBase<CExpression>
    {
    private:
        ExpressionParserType m_parserType;
        bool m_supportsMultiLine;

    public:
        /// @brief Initializes an instance of 'CExpressionParser'
        CExpressionParser(ExpressionParserType parserType, bool supportsMultiline)
            : m_parserType(parserType),
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
