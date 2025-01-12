#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CPhdrsRegionContentParser.h"
#include "CFunctionParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../../Models/CPhdrsStatement.h"
#include "../../Models/CComment.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"
#include "../../Models/Raw/CRawFile.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

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
}

std::shared_ptr<CPhdrsStatement> CPhdrsRegionContentParser::TryParse(
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{   
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    LinqVector<CParsedContentBase> parsedContent;
    LinqVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingName;

    CRawEntry nameEntry;
    CRawEntry typeEntry;
    CRawEntry fileHdrEntry;
    CRawEntry phdrsEntry;

    // This is to parse the AT(<Expression>) part. The expression is parenthesized.
    CFunctionParser functionParser;

    std::shared_ptr<CParsedContentBase> atAddressFunction;
    std::shared_ptr<CParsedContentBase> flagsFunction;
    CRawEntry semicolonEntry;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        // Edge case coverage, where we read end-of-file prematurely.
        if (localIterator == endOfVectorIterator)
        {
            switch (parserState)
            {
            case ParserState::AwaitingName:
            case ParserState::AwaitingType:
                return nullptr;            

            case ParserState::AwaitingOptionalFields:
                violations.emplace_back(std::make_shared<CParserViolation>(nameEntry, EParserViolationCode::PhdrsCommandIsIncomplete));
                --localIterator;
                break;

            case ParserState::AwaitingSemicolon:
                violations.emplace_back(std::make_shared<CParserViolation>(nameEntry, EParserViolationCode::ExpectedSemicolonWasNotFound));
                --localIterator;
                break;

            default:
                throw CMasterParsingException(
                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                    "ParserState invalid in CPhdrsRegionContentParser");
            }
        }

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {          
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Wildcard:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingType:
                    case ParserState::AwaitingOptionalFields:
                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser.");
                }
                break;
            }

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        if (CParserHelpers::IsIllegalProgramHeaderName(resolvedContent))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::ProgramHeaderNameShouldNotBeAReservedKeyword)));
                            parserState = ParserState::AwaitingSemicolon;
                            break;
                        }         

                        nameEntry = *iterator;
                        parserState = ParserState::AwaitingType;
                        break;
                    }
 
                    case ParserState::AwaitingType:
                    {
                        if (!CParserHelpers::IsLegalProgramHeaderType(resolvedContent))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingProgramHeaderTypeHere)));
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
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, fileHdrEntry }, EParserViolationCode::ProgramHeaderFileHdrAlreadySet)));
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
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, phdrsEntry }, EParserViolationCode::ProgramHeaderPhdrsAlreadySet)));
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
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, phdrsEntry }, EParserViolationCode::ProgramHeaderAtAddressAlreadySet)));
                            }
                            else
                            {
                                auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedFunction == nullptr)
                                {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, phdrsEntry }, EParserViolationCode::EntryInvalidOrMisplaced)));
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
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, phdrsEntry }, EParserViolationCode::ProgramHeaderFlagsAlreadySet)));
                            }
                            else
                            {
                                auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedFunction == nullptr)
                                {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, phdrsEntry }, EParserViolationCode::EntryInvalidOrMisplaced)));
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
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::ProgramHeaderTypeNotRecognized)));
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        break;
                    }
 
                    case ParserState::AwaitingSemicolon:
                    {                                                
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        parserState = ParserState::AwaitingSemicolon;
                        break;
                    }
 
                    default:
                        throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown, "ParserState invalid in CPhdrsRegionContentParser");
                }
                break;
            }

            case RawEntryType::Semicolon:
            {
                if (parserState == ParserState::AwaitingSemicolon)
                {
                    semicolonEntry = *iterator;
                    break;
                }

                --localIterator;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::ArithmeticOperator:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:            
            case RawEntryType::Comma:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                parserState = ParserState::AwaitingSemicolon;
                break;
            }

            case RawEntryType::Unknown:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
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
    
        localIterator += ((parserState != ParserState::ParsingComplete) ? 1 : 0);
    }

    std::vector<CRawEntry> rawEntries;    
    std::copy(parsingStartIteratorPosition, localIterator + 1, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CPhdrsStatement>(
                new CPhdrsStatement(nameEntry,
                                    typeEntry,
                                    fileHdrEntry,
                                    atAddressFunction,
                                    flagsFunction,
                                    semicolonEntry,
                                    rawEntries,
                                    violations));
}
