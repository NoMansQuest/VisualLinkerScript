#include "CPhdrsRegionContentParser.h"
#include <vector>
#include <memory>
#include <string>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/CPhdrsStatement.h"
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"
#include "../Models/Raw/CRawFile.h"

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

    std::vector<std::string> ListOfProgramHeaderTypes = {
        "PT_NULL",
        "PT_LOAD",
        "PT_DYNAMIC",
        "PT_INTERP",
        "PT_NOTE",
        "PT_SHLIB",
        "PT_PHDR"        
    };

    std::vector<std::string> ListOfIllegalProgramHeaderNames = {
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

    std::shared_ptr<CLinkerScriptContentBase> atAddressFunction;
    std::shared_ptr<CLinkerScriptContentBase> flagsFunction;
    CRawEntry semicolonEntry;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {          
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        if (CheckIfRawEntryInList(linkerScriptFile, *localIterator, ListOfIllegalProgramHeaderNames))
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
                        if (!CheckIfRawEntryInList(linkerScriptFile, *localIterator, ListOfProgramHeaderTypes))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingProgramHeaderTypeHere));
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        else
                        {
                            typeEntry = *iterator;
                            parserState = ParserState::AwaitingOptionalFields;
                        }

                        break;
                    }
 
                    case ParserState::AwaitingOptionalFields:
                    {                        
                        if (resolvedContent == "FILEHDR")
                        {
                            if (fileHdrEntry.IsPresent())
                            {
                                CViolation detectedViolation({*localIterator, fileHdrEntry }, ViolationCode::ProgramHeaderFileHdrAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                fileHdrEntry = *localIterator;
                            }
                        }
                        else if (resolvedContent == "PHDRS")
                        {
                            if (phdrsEntry.IsPresent())
                            {
                                CViolation detectedViolation({ *localIterator, phdrsEntry }, ViolationCode::ProgramHeaderPhdrsAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                fileHdrEntry = *localIterator;
                            }
                        }
                        else if (resolvedContent == "AT")
                        {
                            if (atAddressFunction != nullptr)
                            {
                                CViolation detectedViolation({ *localIterator, phdrsEntry }, ViolationCode::ProgramHeaderAtAddressAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                auto parsedFunction = this->m_expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedFunction == nullptr)
                                {
                                    CViolation detectedViolation({ *localIterator, phdrsEntry }, ViolationCode::EntryInvalidOrMisplaced);
                                    violations.emplace_back(detectedViolation);
                                }
                                else
                                {
                                    atAddressFunction = parsedFunction;
                                }
                            }
                        }
                        else if (resolvedContent == "FLAGS")
                        {
                            if (atAddressFunction != nullptr)
                            {
                                CViolation detectedViolation({*localIterator, phdrsEntry}, ViolationCode::ProgramHeaderFlagsAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                auto parsedFunction = this->m_expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedFunction == nullptr)
                                {
                                    CViolation detectedViolation({*localIterator, phdrsEntry}, ViolationCode::EntryInvalidOrMisplaced);
                                    violations.emplace_back(detectedViolation);
                                }
                                else
                                {
                                    atAddressFunction = parsedFunction;
                                }
                            }
                        }
                        else
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::ProgramHeaderTypeNotRecognized));
                            parserState = ParserState::AwaitingSemicolon;
                        }

                        break;
                    }
 
                    case ParserState::AwaitingSemicolon:
                    {                                                
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        parserState = ParserState::AwaitingSemicolon;
                    }
 
                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                if (resolvedContent == ";")
                {
                    if (parserState == ParserState::AwaitingSemicolon)
                    {
                        semicolonEntry = *iterator;
                        break;
                    }
                    parserState = ParserState::ParsingComplete;
                }
                else
                {
                    violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                    parserState = ParserState::AwaitingSemicolon;
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
