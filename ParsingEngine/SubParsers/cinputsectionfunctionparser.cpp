#include "CInputSectionFunctionParser.h"
#include "CExpressionParser.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CFunctionCall.h"
#include "../Models/CParameterSeparator.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

namespace
{
    /// @brief CFunctionParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CInputSectionFunction> CInputSectionFunctionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    return nullptr;
}
