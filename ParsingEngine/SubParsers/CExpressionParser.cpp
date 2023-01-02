#include "CExpressionParser.h"
#include <vector>
#include <memory>
#include "../Raw/CRawEntry.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& CDefaultParser::TryParse(std::vector<CRawEntry>::const_iterator& iterator)
{



}