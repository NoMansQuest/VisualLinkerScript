#ifndef CFUNCTION_PARSER_H__
#define CFUNCTION_PARSER_H__

#include <memory>

#include "CSubParserBase.h"
#include "SubParserType.h"

#include "../../Models/CFunctionCall.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Object in charge parsing a full function: <NAME>(<EXPRESSION>)
    class CFunctionParser : public CSubParserBase<CFunctionCall>
    {
    private:
        bool m_onlyCommaAsParamterSeparator;
        bool m_multipleParametersSupported;

    public:
        /// @brief Default constructor
        CFunctionParser(bool onlyCommaAsParameterSeparator = true,
                        bool multipleParametersSupported = false)
            : m_onlyCommaAsParamterSeparator(onlyCommaAsParameterSeparator),
              m_multipleParametersSupported(multipleParametersSupported)
        {}

        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override 
        {
            return SubParserType::FunctionParser;
        }

        /// @copydoc CSubParserBase::TryParse()
        virtual std::shared_ptr<CFunctionCall> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}


#endif
