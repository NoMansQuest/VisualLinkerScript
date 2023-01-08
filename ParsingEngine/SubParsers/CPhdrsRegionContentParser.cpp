
#include "CPhdrsRegionContentParser.h"
#include <vector>
#include <memory>
#include <string>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/CPhdrsRegion.h"
#include "../Models/CPhdrsStatement.h"
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"
#include "../Models/CFunctionCall.h"
#include "../Models/CExpression.h"
#include "../Models/Raw/CRawFile.h"

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
    /// @brief CPhdrsRegionContentParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingType,
        AwaitingOptionalFields,
        AwaitingSemicolon,
        ParsingComplete
    };

    std::vector<std::string> ProgramHeaderTypes = {
        "PT_NULL",
        "PT_LOAD",
        "PT_DYNAMIC",
        "PT_INTERP",
        "PT_NOTE",
        "PT_SHLIB",
        "PT_PHDR"        
    };

    std::vector<std::string> IllegalProgramHeaderNames = {
        "PT_NULL",
        "PT_LOAD",
        "PT_DYNAMIC",
        "PT_INTERP",
        "PT_NOTE",
        "PT_SHLIB",
        "PT_PHDR",
        "FILEHDR",
        "PHDRS",
        "AT",
        "FLAGS",
    };

    /// @brief Checks to see if the content of the @see {rawEntry} (resovled on @see {sourceFile}) is found 
    ///        in the vector of string as pointed out by @see {targetList}.
    const bool CheckIfRawEntryInList(
            const CRawFile& sourceFile,
            const CRawEntry& rawEntry, 
            const std::vector<std::string>& targetList)
    {
        if (rawEntry.EntryType() == RawEntryType::NotPresent)
        {
            return false;
        }

        auto resolvedValue = sourceFile.ResolveRawEntry(rawEntry);
        return std::find(targetList.begin(), targetList.end(), resolvedValue) != targetList.end();
    }
}


std::shared_ptr<CLinkerScriptContentBase> CPhdrsRegionContentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{   
    std::vector<CRawEntry>::const_iterator localIterator = iterator;      
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;    
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;          

    auto parserState = ParserState::AwaitingName;
    auto doNotAdvance = false;

    CRawEntry nameEntry;
    CRawEntry typeEntry;
    CRawEntry fileHdrEntry;
    CRawEntry phdrsEntry;
    std::shared_ptr<CFunctionCall> atAddressFunction;
    std::shared_ptr<CFunctionCall> flagsFunction;
    CRawEntry semicolonEntry;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {          
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},std::vector<CViolation>())));
                break;
            }

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        if (CheckIfRawEntryInList(linkerScriptFile, *localIterator, IllegalProgramHeaderNames))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::ProgramHeaderNameShouldNotBeAReservedKeyword));
                            parserState = ParserState::AwaitingSemicolon;
                            break;
                        }         

                        nameEntry = *iterator;
                        parserState = ParserState::AwaitingType;
                        break;
                    }
 
                    case ParserState::AwaitingType:
                    {
                        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
                        if ((resolvedContent == "FILEHDR") || (resolvedContent == "PHDRS"))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingProgramHeaderTypeHere));
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        else if (!CheckIfRawEntryInList(linkerScriptFile, *localIterator, ProgramHeaderTypes))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::ProgramHeaderTypeNotRecognized));
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        else
                        {
                            nameEntry = *iterator;
                            parserState = ParserState::AwaitingType;
                        }

                        break;
                    }
 
                    case ParserState::AwaitingOptionalFields:
                    {
                        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
                        if (resolvedContent == "FILEHDR")
                        {
                            fileHdrEntry = *localIterator;
                        }
                        else if (resolvedContent == "PHDRS")
                        {
                            doNotAdvance = true;
                        }
                        else if (!CheckIfRawEntryInList(linkerScriptFile, *localIterator, ProgramHeaderTypes))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::ProgramHeaderTypeNotRecognized));
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        else
                        {
                            // Something bad has happened...
                            // nameEntry = *iterator;
                            // parserState = ParserState::AwaitingType;
                        }

                        break;
                    }
 
                    case ParserState::AwaitingSemicolon:
                    {
 
                    }
 
                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                    }
                }


                /*
                if (parserState == ParserState::AwaitingClosingBracket)      
                {   
                    auto parsedStatement = std::move(this->m_phdrsRegionContentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                    if (parsedStatement.size() == 0)
                    {
                        CViolation detectedViolation(
                            std::move(std::vector<CRawEntry>{ *localIterator }),
                            ViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader);
                        violations.emplace_back(detectedViolation);
                    }
                    else
                    {
                        parsedContent.insert(parsedContent.end(), parsedStatement.begin(), parsedContent.end());
                    }
                }
                else if (parserState == ParserState::AwaitingPhdrsHeader)
                {
                    auto stringContent = linkerScriptFile.ResolveRawEntry(*localIterator);
                    if (stringContent != "PHDRS")
                    {
                        // Full abort in this cas
                        return std::move(parsedContent);
                    }

                    phdrHeaderEntry = *localIterator;
                    parserState = ParserState::AwaitingOpenningBracket;
                }
                else if (parserState == ParserState::AwaitingOpenningBracket)
                {                    
                    CViolation detectedViolation(
                        std::move(std::vector<CRawEntry>{ *localIterator }),
                        ViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader);                            

                    violations.emplace_back(std::move(detectedViolation));
                }
                break;*/
            }
            
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
    
        localIterator = ((parserState != ParserState::ParsingComplete) && !doNotAdvance) ?
                        localIterator + 1 : 
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;    
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    // We repot back the position parsing should continue with;
    iterator = (localIterator == endOfVectorIterator) ?
                localIterator :
                localIterator + 1;

    auto phdrsStatement = std::shared_ptr<CLinkerScriptContentBase>(
                new CPhdrsStatement(nameEntry,
                                    typeEntry,
                                    fileHdrEntry,
                                    atAddressFunction,
                                    flagsFunction,
                                    semicolonEntry,
                                    std::move(rawEntries),
                                    std::move(violations)));

    return phdrsStatement;
}
