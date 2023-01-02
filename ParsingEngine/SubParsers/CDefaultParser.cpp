#include "CDefaultParser.h"
#include <vector>
#include <memory>
#include "../Raw/CRawEntry.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

std::vector<std::unique_ptr<CLinkerScriptContentBase>>&& CDefaultParser::TryParse(std::vector<CRawEntry>::const_iterator& iterator)
{
    std::vector<CLinkerScriptContentBase> parsedContent;
    std::vector<CRawEntry> composition {*iterator};         
    std::vector<CRawEntry> violationComposition {*iterator++};         
    std::vector<CViolation> violations {CViolation(std::move(violationComposition), ViolationCode::EntryInvalidOrMisplaced)};    
    parsedContent.emplace_back(CUnrecognizableContent(std::move(composition),std::move(violations)));
    return std::move(parsedContent);
}