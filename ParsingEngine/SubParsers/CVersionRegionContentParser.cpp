#include "CVersionRegionContentParser.h"
#include <vector>
#include <memory>
#include <string>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"
#include "../Models/Raw/CRawFile.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

std::shared_ptr<CVersionScope> CVersionRegionContentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    // TODO: Implement the parser
    return nullptr;
}
