#include "CDefaultParser.h"
#include <vector>
#include <memory>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

std::shared_ptr<CLinkerScriptContentBase> CDefaultParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    CViolation violation({*iterator}, ViolationCode::EntryInvalidOrMisplaced);
    std::vector<CViolation> violations {violation};
    return std::make_shared<CUnrecognizableContent>(std::vector<CRawEntry>{*iterator++}, std::move(violations));
}
