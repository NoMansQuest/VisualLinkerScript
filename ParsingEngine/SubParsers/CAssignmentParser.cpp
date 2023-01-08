#include "CAssignmentParser.h"
#include <vector>
#include "../Models/Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;

std::shared_ptr<CLinkerScriptContentBase> CAssignmentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    // TODO: Implement the parser
    return nullptr;
}
