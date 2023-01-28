#include "CMasterParser.h"
#include "Models/CComment.h"

#include "CMasterParserException.h"
#include "SubParsers/CAssignmentParser.h"
#include "SubParsers/CDefaultParser.h"
#include "SubParsers/CMemoryRegionParser.h"
#include "SubParsers/CPhdrsRegionParser.h"
#include "SubParsers/CSectionsRegionParser.h"
#include "SubParsers/CSubParserBase.h"
#include "SubParsers/CVersionRegionParser.h"
#include "SubParsers/CSectionsRegionOverlayParser.h"

#include "Models/CLinkerScriptContentBase.h"
#include "Models/Raw/RawEntryType.h"

#include <vector>
#include <memory>

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

CMasterParser::CMasterParser()
{    
    this->m_subParsers.emplace_back(std::shared_ptr<CSubParserBase>(new CAssignmentParser()));
    this->m_subParsers.emplace_back(std::shared_ptr<CSubParserBase>(new CPhdrsRegionParser() ));
    this->m_subParsers.emplace_back(std::shared_ptr<CSubParserBase>(new CMemoryParserRegion()));
    this->m_subParsers.emplace_back(std::shared_ptr<CSubParserBase>(new CSectionsRegionParser()));
    this->m_subParsers.emplace_back(std::shared_ptr<CSubParserBase>(new CVersionRegionParser()));
    this->m_subParsers.emplace_back(std::shared_ptr<CSubParserBase>(new CDefaultParser()));
}

CLinkerScriptFile&& CMasterParser::ProcessLinkerScriptFile(std::shared_ptr<CRawFile> rawFile)
{
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    auto iterator = rawFile->Content().cbegin();
    auto iteratorEnding = rawFile->Content().cend();

    while (iterator != rawFile->Content().cend())
    {   
        auto rawEntry = *iterator;
        std::vector<CRawEntry>::const_iterator preParsingIterator = iterator;

        switch (rawEntry.EntryType())
        {
            case RawEntryType::Comment:
            {
                auto comment = std::shared_ptr<CComment>(new CComment({rawEntry},{}));
                parsedContent.emplace_back(comment);
                break;
            }

            case RawEntryType::Word:
            {
                break;
            }

            case RawEntryType::String:
            {

            }

            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:            
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            case RawEntryType::Unknown:
            {
                std::shared_ptr<CLinkerScriptContentBase> returnedContent;
                for (auto parser: this->m_subParsers)
                {                    
                    returnedContent = parser->TryParse( *rawFile, iterator, iteratorEnding);
                    if (returnedContent == nullptr)
                    {
                        continue;
                    }
                }

                if (returnedContent == nullptr)
                {
                    throw CMasterParsingException(MasterParsingExceptionType::NoParserWasAbleToProceed);
                }

                parsedContent.emplace_back(returnedContent);
                break;
            }

            case RawEntryType::NotPresent:
            {
                throw CMasterParsingException(MasterParsingExceptionType::NotPresentEntryDetected);
            }

            default:    
            {
                throw CMasterParsingException(MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound);
            }
        }
    }

    return std::move(CLinkerScriptFile(rawFile, std::move(parsedContent)));
}
