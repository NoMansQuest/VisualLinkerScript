#include <vector>
#include <memory>

#include "CAssignmentParser.h"
#include "CExpressionParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CComment.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/CAssignmentStatement.h"

#include "../CParserViolation.h"
#include "../EParserViolationCode.h"


using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;

namespace
{
    /// @brief CAssignmentParser parser states
    enum class ParserState
    {
        AwaitingLValue,
        AwaitingAssignmentSymbol,
        AwaitingRValueExpression,
        AwaitingSemicolon,
        ParsingComplete
    };
}

std::shared_ptr<CAssignmentStatement> CAssignmentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CParserViolation> violations;

    auto parserState = ParserState::AwaitingLValue;
    auto doNotAdvance = false;
    auto joiningOperatorsObserved = false;

    CRawEntry lValueSymbol;
    CRawEntry assignmentSymbol;
    CRawEntry semicolonOperatorEntry;

    std::shared_ptr<CExpression> parsedRValue;
    CExpressionParser rValueExpressionParser(ExpressionParserType::NormalParser, false);

    if (localIterator->EntryType() != RawEntryType::Word)
    {
        // Parsing failed...
        return nullptr;
    }

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
            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        if (localIterator->EntryType() == RawEntryType::Word)
                        {
                            lValueSymbol = *localIterator;
                            parserState = ParserState::AwaitingAssignmentSymbol;
                            break;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    case ParserState::AwaitingAssignmentSymbol:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedRValue);
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingAssignmentSymbol:
                    case ParserState::AwaitingRValueExpression:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        if (this->m_isInsideProcedure)
                        {
                            localIterator = previousPositionIterator;
                            parserState = ParserState::ParsingComplete;
                        }
                        else
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
            }

            case RawEntryType::Operator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        // We can't proceed, we should have been starting with a word
                        return nullptr;
                    }

                    case ParserState::AwaitingAssignmentSymbol:
                    {
                        if (resolvedContent == ":")
                        {
                            // This could be a "Section" definition in the "Sections" region. We may need to abort.
                            if (joiningOperatorsObserved)
                            {
                                // We don't abort, this is a R-Value expression put in L-Value
                                violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::LValueCannotContainRValueExpression));
                            }
                            else
                            {
                                // Abort, we're going up the wrong ladder.
                                return nullptr;
                            }
                        }
                        else if (CParserHelpers::IsArithmeticOperator(resolvedContent))
                        {
                            joiningOperatorsObserved = true;
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::LValueCannotContainRValueExpression));
                        }
                        else
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        if (resolvedContent == ";" && !this->m_isInsideProcedure)
                        {
                            semicolonOperatorEntry = *localIterator;
                            parserState = ParserState::ParsingComplete;
                        }
                        else
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
                break;
            }

            case RawEntryType::Assignment:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::MissingLValue));
                        break;
                    }

                    case ParserState::AwaitingAssignmentSymbol:
                    {
                        assignmentSymbol = *localIterator;
                        parserState = ParserState::AwaitingRValueExpression;
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::MultipleAssignmentOperatorsDetected));
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
            }

            case RawEntryType::Number:
            case RawEntryType::String:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingAssignmentSymbol:
                    case ParserState::AwaitingLValue:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::LValueCannotContainRValueExpression));
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingAssignmentSymbol:
                    case ParserState::AwaitingLValue:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingRValueExpression:
                    case ParserState::AwaitingSemicolon:
                    {
                        // Need to mark this error
                        localIterator = previousPositionIterator;
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::RValueExpressionParsingFailed));
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }

                break;
            }

            case RawEntryType::Unknown:
            {
                violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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

        previousPositionIterator = localIterator;
        localIterator = ((parserState != ParserState::ParsingComplete) && !doNotAdvance) ?
                        localIterator + 1 :
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CAssignmentStatement>(
                new CAssignmentStatement(lValueSymbol,
                                         assignmentSymbol,
                                         semicolonOperatorEntry,
                                         std::move(parsedContent),
                                         std::move(rawEntries),
                                         std::move(violations)));
}
