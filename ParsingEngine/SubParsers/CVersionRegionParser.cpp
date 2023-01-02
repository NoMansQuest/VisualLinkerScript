#include "CVersionRegionParser.h"
#include <vector>
#include <memory>
#include "../Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;

std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& CVersionRegionParser::TryParse(std::vector<CRawEntry>::const_iterator& iterator)
{
    std::vector<CLinkerScriptContentBase> parsedContent;

    // TODO: Implement the parser

    return std::move(parsedContent);
}