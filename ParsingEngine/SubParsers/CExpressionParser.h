#ifndef CEXPRESSION_PARSER_H__
#define CEXPRESSION_PARSER_H__

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CExpression.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge parsing R-Value expression (e.g. a + b - c + ((f / g) - h) ...)
    class CExpressionParser : public CSubParserBase<CExpression>
    {
    private:
        bool m_supportsMultiLine;
        bool m_isParenthesizedExpressionParser;
        bool m_isSectionOutputAddressParser;

    public:
        /// @brief Initializes an instance of 'CExpressionParser'
        CExpressionParser(bool supportsMultiline,
                          bool isParenthesizedExpressionParser,
                          bool isSectionOutputAddressParser)
            : m_supportsMultiLine(supportsMultiline),
              m_isParenthesizedExpressionParser(isParenthesizedExpressionParser),
              m_isSectionOutputAddressParser(isSectionOutputAddressParser)
        {}

        /// @brief Initializes an instance of 'CExpressionParser'
        CExpressionParser()
            : m_supportsMultiLine(false),
              m_isParenthesizedExpressionParser(false),
              m_isSectionOutputAddressParser(false)
        {}

        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::ExpressionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CExpression> TryParse(
                const CLinkerScriptFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator endOfVectorIterator) override;
    };
}

#endif
