
#include "CPhdrsRegionParser.h"
#include <vector>
#include <memory>
#include <string>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/CPhdrsRegion.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"
#include "../CMasterParserException.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace
{
    /// @brief 
    enum class ParserState
    {
        AwaitingPhdrsHeader,
        AwaitingOpenningBracket,        
        AwaitingClosingBracket,
        ParsingComplete
    };
}

std::shared_ptr<CLinkerScriptContentBase> CPhdrsRegionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;      
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;    
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;          

    auto parserState = ParserState::AwaitingPhdrsHeader;

    CRawEntry phdrHeaderEntry;
    CRawEntry openningBracketEntry;
    CRawEntry closingBracketEntry;

    while ((localIterator != endOfVectorIterator) && 
           (parserState != ParserState::ParsingComplete))
    {
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment(std::vector<CRawEntry>{*localIterator}, std::vector<CViolation>())));
                break;
            }

            case RawEntryType::Word:
            {
                if (parserState == ParserState::AwaitingClosingBracket)      
                {   
                    auto parsedStatement = this->m_phdrsRegionContentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                    if (parsedStatement == nullptr)
                    {
                        CViolation detectedViolation(
                            std::vector<CRawEntry>{ *localIterator },
                            ViolationCode::EntryInvalidOrMisplaced);
                        violations.emplace_back(detectedViolation);
                    }
                    else
                    {
                        parsedContent.emplace_back(parsedStatement);
                    }
                }
                else if (parserState == ParserState::AwaitingPhdrsHeader)
                {
                    auto stringContent = linkerScriptFile.ResolveRawEntry(*localIterator);
                    if (stringContent != "PHDRS")
                    {
                        // Full abort in this cas
                        return nullptr;
                    }

                    phdrHeaderEntry = *localIterator;
                    parserState = ParserState::AwaitingOpenningBracket;
                }
                else if (parserState == ParserState::AwaitingOpenningBracket)
                {                    
                    CViolation detectedViolation(
                        std::vector<CRawEntry>{ *localIterator },
                        ViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader);                            

                    violations.emplace_back(std::move(detectedViolation));
                }
                break;
            }
            
            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            {                
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    CViolation detectedViolation(
                            std::vector<CRawEntry>{ *localIterator },
                            ViolationCode::EntryInvalidOrMisplaced);
                }
                else if (parserState == ParserState::AwaitingOpenningBracket)
                {
                    CViolation detectedViolation(
                            std::vector<CRawEntry>{ *localIterator },
                            ViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader);                            

                    violations.emplace_back(std::move(detectedViolation));
                }
                else if (parserState == ParserState::AwaitingPhdrsHeader)
                {
                    parserState = ParserState::ParsingComplete;
                }
                break;
            }

            case RawEntryType::BracketOpen:
            {
                if (parserState == ParserState::AwaitingOpenningBracket)
                {
                    openningBracketEntry = *localIterator;
                    parserState = ParserState::AwaitingClosingBracket;
                }
                else if ((parserState == ParserState::AwaitingClosingBracket) ||
                         (parserState == ParserState::AwaitingPhdrsHeader))
                {
                    parserState = ParserState::ParsingComplete;
                }
                break;
            }

            case RawEntryType::BracketClose:
            {
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    closingBracketEntry = *localIterator;
                    parserState = ParserState::ParsingComplete;                    
                }                
                break;
            }

            case RawEntryType::Unknown:
            {
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected, 
                        "A 'Unknown' entry was detected.");
            }

            case RawEntryType::NotPresent:
            {
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected, 
                        "A 'non-present' entry was detected.");
            }

            default:
            {
                throw CMasterParsingException(
                        MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound, 
                        "Unrecognized raw-entry type detected.");
            }
        }        
    
        localIterator = (parserState != ParserState::ParsingComplete) ? 
                        localIterator + 1 : 
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;    
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    auto phdrsRegion = std::shared_ptr<CLinkerScriptContentBase>(
                new CPhdrsRegion(phdrHeaderEntry,
                                 openningBracketEntry,
                                 closingBracketEntry,
                                 std::move(parsedContent),
                                 std::move(rawEntries),
                                 std::move(violations)));

    // We repot back the position parsing should continue with;
    iterator = (localIterator == endOfVectorIterator) ? 
                localIterator : 
                localIterator + 1; 

    return phdrsRegion;
}
