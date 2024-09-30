#include <vector>
#include <memory>
#include <iterator>

#include "CAssignmentParser.h"
#include "CExpressionParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CLinkerScriptFile.h"
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
        AwaitingAssignmentOperator,
        AwaitingRValueExpression,
        AwaitingSemicolon,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CAssignmentStatement> CAssignmentParser::TryParse(
        const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    SharedPtrVector<CParsedContentBase> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingLValue;
    auto doNotAdvance = false;
    auto joiningOperatorsObserved = false;

    CRawEntry lValueSymbol;
    CRawEntry assignmentSymbol;
    CRawEntry semicolonOperatorEntry;

    std::shared_ptr<CExpression> parsedRValue;
    CExpressionParser rValueExpressionParser;
    auto finalParserState = this->m_isInsideProcedure ? ParserState::AwaitingParenthesisClosure : ParserState::AwaitingSemicolon;

    if (localIterator->EntryType() != RawEntryType::Word)
    {
        // Parsing failed...
        return nullptr;
    }

    while (parserState != ParserState::ParsingComplete)
    {
        // Edge case coverage, where we read end-of-file prematurely.
        if (localIterator == endOfVectorIterator)
        {
            switch (parserState)
            {
	            case ParserState::AwaitingLValue:
	            case ParserState::AwaitingAssignmentOperator:
	                return nullptr;

	            case ParserState::AwaitingRValueExpression:
	            case ParserState::AwaitingSemicolon:
	            case ParserState::AwaitingParenthesisClosure:
	            {
	                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(lValueSymbol, EParserViolationCode::RValueExpressionMissing)));
                    --localIterator;
                    break;
	            }
	            default:
	                throw CMasterParsingException(
	                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
	                    "ParserState invalid in CAssignmentParser");
            }            
        }

        // General parsing logic.
        auto rawEntry = *localIterator;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(rawEntry);

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
                    case ParserState::AwaitingLValue:
                    case ParserState::AwaitingAssignmentOperator:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingRValueExpression:
                    case ParserState::AwaitingSemicolon:
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                        break;
                    }
                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
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
                            parserState = ParserState::AwaitingAssignmentOperator;
                            break;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    case ParserState::AwaitingAssignmentOperator:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedRValue);
                            parserState = finalParserState;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingAssignmentOperator:
                    case ParserState::AwaitingRValueExpression:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        localIterator--;
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

            case RawEntryType::Semicolon:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        // We can't proceed, we should have been starting with a word
                        return nullptr;
                    }

                    case ParserState::AwaitingAssignmentOperator:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedRValue);
                            parserState = (this->m_isInsideProcedure) ? ParserState::AwaitingParenthesisClosure : ParserState::AwaitingSemicolon;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        semicolonOperatorEntry = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
                break;
            }

            case RawEntryType::Colon:
            case RawEntryType::Comma:
            case RawEntryType::QuestionMark:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::ArithmeticOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        // We can't proceed, we should have been starting with a word
                        return nullptr;
                    }

                    case ParserState::AwaitingAssignmentOperator:
                    {
                        if (CParserHelpers::IsColon(resolvedContent))
                        {
                            // This could be a "Section" definition in the "Sections" region. We may need to abort.
                            if (joiningOperatorsObserved)
                            {
                                // We don't abort, this is a R-Value expression put in L-Value
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::LValueCannotContainRValueExpression)));
                            }
                            else
                            {
                                // Abort, we're going up the wrong ladder.
                                return nullptr;
                            }
                            break;
                        }

                        if (CParserHelpers::IsArithmeticOperator(resolvedContent) ||
                            CParserHelpers::IsComparisonOperator(resolvedContent) ||
                            CParserHelpers::IsLogicalOperator(resolvedContent))
                        {
                            joiningOperatorsObserved = true;
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::LValueCannotContainRValueExpression)));
                        }
                        else
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedRValue);
                            parserState = finalParserState;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
                break;
            }

            case RawEntryType::AssignmentOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingLValue)));
                        break;
                    }

                    case ParserState::AwaitingAssignmentOperator:
                    {
                        assignmentSymbol = *localIterator;
                        parserState = ParserState::AwaitingRValueExpression;
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MultipleAssignmentOperatorsDetected)));
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentParser");
                }
                break;
            }

            case RawEntryType::Number:
            case RawEntryType::String:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingAssignmentOperator:
                    case ParserState::AwaitingLValue:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::LValueCannotContainRValueExpression)));
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedRValue);
                            parserState = finalParserState;
                        }
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
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
                    case ParserState::AwaitingAssignmentOperator:
                    case ParserState::AwaitingLValue:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingRValueExpression:
                    case ParserState::AwaitingSemicolon:
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        // Need to mark this error
                        localIterator--;
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::RValueExpressionParsingFailed)));
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

    return std::shared_ptr<CAssignmentStatement>(
                new CAssignmentStatement(lValueSymbol,
                                         assignmentSymbol,
                                         semicolonOperatorEntry,
                                         parsedContent,
                                         rawEntries,
                                         violations));
}
