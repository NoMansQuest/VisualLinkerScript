#include <vector>
#include <memory>
#include <string>

#include "CPhdrsRegionContentParser.h"
#include "CExpressionParser.h"
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
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{   
    std::vector<CRawEntry>::const_iterator localIterator = iterator;      
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;    
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingName;
    auto doNotAdvance = false;

    CRawEntry nameEntry;
    CRawEntry typeEntry;
    CRawEntry fileHdrEntry;
    CRawEntry phdrsEntry;

    CExpressionParser expressionParser(ExpressionParserType::NormalParser, false);

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
                                auto parsedFunction = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
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
                                auto parsedFunction = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
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
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                    parserState = ParserState::AwaitingSemicolon;
                }
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
    
        localIterator = ((parserState != ParserState::ParsingComplete) && !doNotAdvance) ?
                        localIterator + 1 : 
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;    
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CPhdrsStatement>(
                new CPhdrsStatement(nameEntry,
                                    typeEntry,
                                    fileHdrEntry,
                                    atAddressFunction,
                                    flagsFunction,
                                    semicolonEntry,
                                    std::move(rawEntries),
                                    std::move(violations)));
}
