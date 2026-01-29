#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "ParsingEngine/SubParsers/CExpressionParser.h"
#include "ParsingEngine/SubParsers/CMemoryRegionContentParser.h"
#include "ParsingEngine/SubParsers/CMemoryStatementAttributeParser.h"
#include "ParsingEngine/SubParsers/Constants.h"

#include "ParsingEngine/EParserViolationCode.h"
#include "ParsingEngine/CMasterParserException.h"
#include "ParsingEngine/CParserViolation.h"
#include "Models/CComment.h"
#include "Models/CMemoryStatement.h"
#include "Models/Raw/CRawEntry.h"
#include "Models/Raw/CRawFile.h"
#include "Models/Raw/RawEntryType.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

namespace
{
    /// @brief CMemoryRegionContentParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingAttributes,
        AwaitingColon,
        AwaitingOriginHeader,
        AwaitingOriginAssignmentSymbol,
        AwaitingOriginRValue,
        AwaitingLengthHeader,
        AwaitingLengthAssignmentSymbol,
        AwaitingLengthRValue,
        AwaitingCommaSeparatingOriginAndLength,
        ParsingComplete
    };
}

std::shared_ptr<CMemoryStatement> CMemoryRegionContentParser::TryParse(
        const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;

    LinqVector<CParsedContentBase> parsedContent;
    LinqVector<CViolationBase> violations;   
    CMemoryStatementAttributeParser attributeParser;

    auto parserState = ParserState::AwaitingName;

    CRawEntry nameEntry;
    CRawEntry colonEntry;
    CRawEntry originHeaderEntry;
    CRawEntry originAssignmentSymbol;
    CRawEntry commaSeparatingOriginAndLength;
    CRawEntry lengthHeaderEntry;
    CRawEntry lengthAssignmentSymbol;
    std::shared_ptr<CMemoryStatementAttribute> attributes;
    std::shared_ptr<CExpression> originRValue;
    std::shared_ptr<CExpression> lengthRValue;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        // Edge case coverage, where we read end-of-file prematurely.
        if (localIterator == endOfVectorIterator)
        {
            switch (parserState)
            {
            case ParserState::AwaitingName:
            case ParserState::AwaitingAttributes:
            case ParserState::AwaitingColon:
                return nullptr;
            
            case ParserState::AwaitingOriginHeader:
            case ParserState::AwaitingOriginAssignmentSymbol:
            case ParserState::AwaitingOriginRValue:
            case ParserState::AwaitingLengthHeader:
            case ParserState::AwaitingLengthAssignmentSymbol:
            case ParserState::AwaitingLengthRValue:
            case ParserState::AwaitingCommaSeparatingOriginAndLength:     
                violations.emplace_back(std::make_shared<CParserViolation>(nameEntry, EParserViolationCode::IncompleteMemoryDefinition));
                --localIterator;
                break;

            default:
                throw CMasterParsingException(
                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                    "ParserState invalid in CInputSectionStatementParser");
            }
        }

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        if (lineChangeDetected)
        {
            // Any line-change would be rendered parsing attempt null and void (however, it may be possible to report
            // back a type of statement)
            break;
        }

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

                    case ParserState::AwaitingAttributes:
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingOriginRValue:
                    case ParserState::AwaitingLengthHeader:
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingLengthRValue:
                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser.");
                }
                break;
            }

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MemorySectionNameShouldNotBeAReservedKeyword)));
                        }

                        nameEntry = *iterator;
                        parserState = ParserState::AwaitingAttributes;
                        break;
                    }

                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingAttributes:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingAttributeHere)));
                        break;
                    }

                    case ParserState::AwaitingOriginRValue:
                    {
                        CExpressionParser expressionParser(false, false, false);
                        auto parsedRValue = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            originRValue = parsedRValue;
                            parserState = ParserState::AwaitingCommaSeparatingOriginAndLength;
                        }
                        break;
                    }

                    case ParserState::AwaitingLengthRValue:
                    {
                        CExpressionParser expressionParser(false, false, false);
                        auto parsedRValue = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            lengthRValue = parsedRValue;
                            parserState = ParserState::ParsingComplete;
                        }
                        break;
                    }

                    case ParserState::AwaitingOriginHeader:
                    {                       
                        if (!StringEquals(resolvedContent, "ORIGIN", true))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingOriginDeclaration)));
                        }
                        else
                        {
                            originHeaderEntry = *localIterator;
                            parserState = ParserState::AwaitingOriginAssignmentSymbol;
                        }
                        break;
                    }

                    case ParserState::AwaitingLengthHeader:
                    {
                        if (!StringEquals(resolvedContent, "LEN", true) &&
                            !StringEquals(resolvedContent, "l", true) &&
                            !StringEquals(resolvedContent, "LENGTH", true))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingLengthDeclaration)));
                        }
                        else
                        {
                            lengthHeaderEntry = *localIterator;
                            parserState = ParserState::AwaitingLengthAssignmentSymbol;
                        }
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::Comma:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    {
                        commaSeparatingOriginAndLength = *localIterator;
                        parserState = ParserState::AwaitingLengthHeader;
                        break;
                    }

                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingAttributes:
                    case ParserState::AwaitingOriginRValue:
                    case ParserState::AwaitingLengthRValue:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingLengthHeader:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::Number:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingOriginRValue:
                    {
                        CExpressionParser expressionParser(false, false, false);
                        auto parsedRValue = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            originRValue = parsedRValue;
                            parserState = ParserState::AwaitingCommaSeparatingOriginAndLength;
                        }
                        break;
                    }

                    case ParserState::AwaitingLengthRValue:
                    {
                        CExpressionParser expressionParser(false, false, false);
                        auto parsedRValue = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            lengthRValue = parsedRValue;
                            parserState = ParserState::ParsingComplete;
                        }
                        break;
                    }

                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingAttributes:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingLengthHeader:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::AssignmentOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    {
                        if (resolvedContent != "=")
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::CompoundAssignmentOperatorNotAllowedHere)));
                        }

                        lengthAssignmentSymbol = *localIterator;
                        parserState = ParserState::AwaitingLengthRValue;
                        break;
                    }

                    case ParserState::AwaitingOriginAssignmentSymbol:
                    {
                        if (resolvedContent != "=")
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::CompoundAssignmentOperatorNotAllowedHere)));
                        }

                        originAssignmentSymbol = *localIterator;
                        parserState = ParserState::AwaitingOriginRValue;
                        break;
                    }

                    case ParserState::AwaitingOriginRValue:
                    case ParserState::AwaitingLengthRValue:
                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingAttributes:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingLengthHeader:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingEqualOperatorHere)));
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::Colon:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingColon:
                    {
                        colonEntry = *localIterator;
                        parserState = ParserState::AwaitingOriginHeader;
                        break;
                    }

                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingOriginRValue:
                    case ParserState::AwaitingLengthRValue:
                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingAttributes:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingLengthHeader:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::ArithmeticOperator:
            {
                // Under certain circumstances, we might be able to parse an expression here.
                if (!StringEquals(resolvedContent, "+") && !StringEquals(resolvedContent, "-"))
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                    break;
                }

                // Give it a try...
                switch (parserState)
                {
                    case ParserState::AwaitingOriginRValue:
                    {
                        CExpressionParser expressionParser(false, false, false);
                        auto parsedRValue = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            originRValue = parsedRValue;
                            parserState = ParserState::AwaitingCommaSeparatingOriginAndLength;
                        }
                        break;
                    }

                    case ParserState::AwaitingLengthRValue:
                    {
                        CExpressionParser expressionParser(false, false, false);
                        auto parsedRValue = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            lengthRValue = parsedRValue;
                            parserState = ParserState::ParsingComplete;
                        }
                        break;
                    }

                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingAttributes:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingLengthHeader:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::Semicolon:
            case RawEntryType::QuestionMark:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::String:            
            case RawEntryType::ParenthesisClose:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingAttributes:
                    {
                        auto parsedAttribute = attributeParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedAttribute == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            attributes = parsedAttribute;
                            parserState = ParserState::AwaitingColon;
                        }
                        break;
                    }

                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingLengthAssignmentSymbol:
                    case ParserState::AwaitingOriginAssignmentSymbol:
                    case ParserState::AwaitingOriginRValue:
                    case ParserState::AwaitingLengthRValue:
                    case ParserState::AwaitingCommaSeparatingOriginAndLength:
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingOriginHeader:
                    case ParserState::AwaitingLengthHeader:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryRegionContentParser");
                    }
                }
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                localIterator--;
                parserState = ParserState::ParsingComplete;
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

    if (!nameEntry.IsPresent() || !colonEntry.IsPresent())
    {
        // It is safe to say that we have failed parsing this statement. It's malformed beyond recognition or
        // outright invalid.
        return nullptr;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator + 1, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CMemoryStatement>(
                new CMemoryStatement(nameEntry,
                                     attributes,
                                     colonEntry,
                                     originHeaderEntry,
                                     originAssignmentSymbol,
                                     commaSeparatingOriginAndLength,
                                     lengthHeaderEntry,
                                     lengthAssignmentSymbol,
                                     originRValue,
                                     lengthRValue,
                                     std::move(rawEntries),
                                     std::move(violations)));
}
