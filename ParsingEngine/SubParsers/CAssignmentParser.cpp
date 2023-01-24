#include "CAssignmentParser.h"
#include <vector>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CAssignmentStatement.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;

std::shared_ptr<CAssignmentStatement> CAssignmentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    // TODO: Implement the parser
    return nullptr;
}
