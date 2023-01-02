#include "CMasterParser.h"
#include "CComment.h"

#include "CMasterParserException.h"
#include "SubParsers/CAssignmentParser.h"
#include "SubParsers/CDefaultParser.h"
#include "SubParsers/CMemoryRegionParser.h"
#include "SubParsers/CPhdrsRegionParser.h"
#include "SubParsers/CSectionsRegionParser.h"
#include "SubParsers/CSubParserBase.h"
#include "SubParsers/CVersionRegionParser.h"
#include "SubParsers/CSectionsOverlayParser.h"

#include "Models/CLinkerScriptContentBase.h"
#include "Models/Raw/RawEntryType.h"

#include <vector>
#include <memory>

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

CMasterParser::CMasterParser()
{
    this->m_subParsers.emplace_back(
            std::make_shared<CAssignmentParser>(),    
            std::make_shared<CPhdrsRegionParser>(),
            std::make_shared<CMemoryParserRegion>(),
            std::make_shared<CSectionsRegionParsers>(),
            std::make_shared<CSectionsRegionOVerlayParsers>(),
            std::make_shared<CVersionRegionParser>(),
            std::make_shared<CDefaultParser>());
}

CLinkerScriptFile&& ProcessLinkerScriptFile(std::shared_ptr<CRawFile> rawFile)
{
    std::vector<std::unique_ptr<CLinkerScriptContentBase>> parsedContent;    
    auto iterator = rawFile->Content().cbegin();

    while (iterator != rawFile->Content().cend())
    {   
        auto preParsingIterator = iterator;
        switch (rawEntry.EntryType())
        {
            case RawEntryType::Word:
            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            case RawEntryType::Unknown:
            {
                for (auto parser: this->m_subParsers)
                {                    
                    auto returnedContent = std::move(parser->TryParse(iterator));                
                    if (parser->)
                    parsedContent.insert(parsedContent.end(), returnedContent.begin(), returnedContent.end());
                }
                break;
            }

            case RawEntryType::NotPresent:
            {
                throw CMasterParserException(MasterParsingExceptionType::NotPresentEntryDetected)
            }

            case RawEntryType::Comment:
            {
                deducedContent.emplace_back(std::move(CComment({rawEntry}, {})));
                break;            
            }

            default:    
            {
                throw CMasterParserException(MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound)
            }
        }    

        if (preParsingIterator == iterator)    
            throw CMasterParserException(MasterParsingExceptionType::NoParserWasAbleToProceed)
    }

    return std::move(CLinkerScriptFile(rawFile, std::move(parsedContent)));
}