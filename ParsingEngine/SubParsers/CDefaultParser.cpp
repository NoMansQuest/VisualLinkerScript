#include "CDefaultParser.h"
#include <vector>
#include <memory>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

std::shared_ptr<CUnrecognizableContent> CDefaultParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    UNREFERENCED_PARAMETER(linkerScriptFile);
    CViolation violation({*iterator}, ViolationCode::EntryInvalidOrMisplaced);
    return std::shared_ptr<CUnrecognizableContent>(new CUnrecognizableContent({*iterator++}, {violation}));
}
