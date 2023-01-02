#include "CAssignmentParser.h"
#include <vector>
#include "../Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;

std::vector<CLinkerScriptContentBase>&& CAssignmentParser::TryParse(std::vector<CRawEntry>::const_iterator& iterator)
{
    std::vector<CLinkerScriptContentBase> parsedContent;

    // TODO: Implement the parser

    return std::move(parsedContent);
}