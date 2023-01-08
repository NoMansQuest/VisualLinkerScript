
#include "CPhdrsRegionParser.h"
#include <vector>
#include <memory>
#include <string>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/CPhdrsRegion.h"
#include "../Models/CPhdrsStatement.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"
#include "../CMasterParserException.h"

/* PHDRS Region has the following format:
*
* PHDRS 
* {
*    name type [ FILEHDR ] [ PHDRS ] [ AT ( address ) ] [ FLAGS ( flags ) ] ;
* }
* 
* 
* @ "type" can have the following potential values: 
* 
* PT_NULL (0):    Indicates an unused program header.
* PT_LOAD (1):    Indicates that this program header describes a segment to be loaded from the file.
* PT_DYNAMIC (2): Indicates a segment where dynamic linking information can be found.
* PT_INTERP (3):  Indicates a segment where the name of the program interpreter may be found.
* PT_NOTE (4):    Indicates a segment holding note information.
* PT_SHLIB (5):   A reserved program header type, defined but not specified by the ELF ABI.
* PT_PHDR (6):    Indicates a segment where the program headers may be found.
* PT_TLS (7):     Indicates a segment containing thread local storage.
* 
* 
* @ The FILEHDR keyword means that the segment should include the ELF file header. 
* @ The PHDRS keyword means that the segment should include the ELF program headers themselves. 
*   -> If applied to a loadable segment (PT_LOAD), all prior loadable segments must have one of these keywords.
* @ The AT command used as an output section attribute. The AT command for a program header overrides the output section attribute.
* @ The value of flags must be an integer. It is used to set the p_flags field of the program header.
* 
*/

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
                            ViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader);
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

    auto phdrsRegion = std::make_shared<CPhdrsRegion>(
                phdrHeaderEntry,
                openningBracketEntry,
                closingBracketEntry,
                std::move(parsedContent),
                std::move(rawEntries),
                std::move(violations));

    // We repot back the position parsing should continue with;
    iterator = (localIterator == endOfVectorIterator) ? 
                localIterator : 
                localIterator + 1; 

    return phdrsRegion;
}
