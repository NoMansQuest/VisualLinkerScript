
#include <vector>
#include <memory>
#include <iterator>

#include "CExpressionParser.h"
#include "CFunctionParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"

#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/CComment.h"
#include "../../Models/CExpression.h"
#include "../../Models/CSymbol.h"
#include "../../Models/CExpressionNumber.h"
#include "../../Models/CExpressionOperator.h"
#include "../../Models/CStringEntry.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;

namespace
{
    /// @brief CExpressionParser parser states
    enum class ParserState
    {
        AwaitingContent,
        AwaitingParenthesisClosure,
        ParsingComplete
    };

    /// @brief CExpressionParser ternary operation states
    enum class TernaryParserState
    {
        AwaitingThenExpression,
        AwaitingElseExpression,
    };
}

std::shared_ptr<CExpression> CExpressionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingContent;
    auto ternaryParserState = TernaryParserState::AwaitingThenExpression;
    auto doNotAdvance = false;

    auto precedingOperatorSet = false;
    auto isFirstEntry = false;

    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        CRawEntry oneEntryAhead;
        if (localIterator + 1 != endOfVectorIterator)
        {
            oneEntryAhead = *(localIterator+1);
        }

        if (lineChangeDetected && !this->m_supportsMultiLine)
        {
            if (precedingOperatorSet)
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnexpectedTerminationOfExpression)));
            }
            // Any line-change would be rended parsing attempt null and void (however, it may be possible to report
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

            case RawEntryType::Word:
            {
                if (!precedingOperatorSet && !isFirstEntry)
                {
                    if (!parenthesisOpen.IsPresent())
                    {
                        // We need to end our parsing here. This is because no preceding operator
                        // was found, and we're not expecting a parenthesis closure either
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    // Each entry must be preceded by an operator (except the first entry)
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingOperatorInRValueExpression)));
                }

                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisClosure:
                    case ParserState::AwaitingContent:
                    {
                        if (CParserHelpers::IsFunctionName(resolvedContent))
                        {
                            if (oneEntryAhead.EntryType() != RawEntryType::ParenthesisOpen)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                                break;
                            }

                            CFunctionParser functionParser(true, true);
                            auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedFunction == nullptr)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                break;
                            }

                            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedFunction));
                        }
                        else if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            // We have an invalid statement here
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            break;
                        }
                        else
                        {
                            auto symbol = std::shared_ptr<CLinkerScriptContentBase>(new CSymbol(*localIterator, {*localIterator}, {}));
                            parsedContent.emplace_back(symbol);
                        }

                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                }

                precedingOperatorSet = false; // Previous operator is already considered to be assigned to this entry;
                break;
            }

            case RawEntryType::Semicolon:
            case RawEntryType::Comma:
            {
                if (precedingOperatorSet)
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingASymbolOrNumberBeforeExpressionEnds)));
                }

                // Forced completion of parsing
                localIterator = previousPositionIterator;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Colon:
            case RawEntryType::QuestionMark:
            case RawEntryType::ArithmeticOperator:
            {
                if (CParserHelpers::IsComparisonOperator(resolvedContent))
                {
                    auto expressionOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                    parsedContent.emplace_back(expressionOperator);
                }
                else if (CParserHelpers::IsTernaryOperator(resolvedContent))
                {
                    switch (ternaryParserState)
                    {
                        case TernaryParserState::AwaitingThenExpression:
                        {
                            if (CParserHelpers::IsColon(resolvedContent))
                            {
                                // We terminate here. This colon could be related to something else...
                                parserState = ParserState::ParsingComplete;
                                localIterator = previousPositionIterator;
                            }
                            else
                            {
                                // Operator is question-mark, which is valid
                                CExpressionParser ternaryThenExpressionParser(this->m_supportsMultiLine, RawEntryType::Colon);
                                auto parserPostQuestionMarkExpression = ternaryThenExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parserPostQuestionMarkExpression == nullptr)
                                {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                }
                                else
                                {
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parserPostQuestionMarkExpression));
                                    ternaryParserState = TernaryParserState::AwaitingElseExpression;
                                }
                            }
                            break;
                        }

                        case TernaryParserState::AwaitingElseExpression:
                        {
                            if (CParserHelpers::IsQuestionMark(resolvedContent))
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                ternaryParserState = TernaryParserState::AwaitingThenExpression;
                            }
                            else
                            {
                                // Operator is a colon, which is valid
                                CExpressionParser ternaryElseExpressionParser(this->m_supportsMultiLine, this->m_delimitingEntry);
                                auto parserPostColonExpression = ternaryElseExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parserPostColonExpression == nullptr)
                                {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                }
                                else
                                {
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parserPostColonExpression));
                                }
                            }

                            // We circle back, as more ternary operations are allowed to appear.
                            ternaryParserState = TernaryParserState::AwaitingThenExpression;
                            break;
                        }

                        default:
                            throw CMasterParsingException(
                                        MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                        "ParserState invalid in CPhdrsRegionContentParser");
                    }
                }
                else
                {
                    if (CParserHelpers::IsComparisonOperator(resolvedContent) ||
                        CParserHelpers::IsArithmeticOperator(resolvedContent) )
                    {
                        auto expressionOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator (*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(expressionOperator);
                        precedingOperatorSet = true; // This allows next found item to be accepted without violaiton
                    }
                    else
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::OperatorIsNotAcceptedHere)));
                    }
                }
                break;
            }

            case RawEntryType::AssignmentOperator:
            {
                // This indicates that we've not been parsing an expression all along
                return nullptr;
            }

            case RawEntryType::Number:
            {
                if (!precedingOperatorSet && !isFirstEntry)
                {
                    if (!parenthesisOpen.IsPresent())
                    {
                        // We need to end our parsing here. This is because no preceding operator
                        // was found, and we're not expecting a parenthesis closure either
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    // Each entry must be preceded by an operator (except the first entry)
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingOperatorInRValueExpression)));
                }

                auto numericValue = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionNumber(*localIterator, {*localIterator}, {}));
                parsedContent.emplace_back(numericValue);
                precedingOperatorSet = false; // Previous operator is already considered to be assigned to this entry;
                break;
            }

            case RawEntryType::String:
            {
                // Strings are not allowd unless they are the first entry
                if (isFirstEntry)
                {
                    auto stringValue = std::shared_ptr<CLinkerScriptContentBase>(new CStringEntry(*localIterator, {*localIterator}, {}));
                    parsedContent.emplace_back(stringValue);
                }
                else
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingOperatorInRValueExpression)));
                }

                precedingOperatorSet = false; // Previous operator is already considered to be assigned to this entry;
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisClosure:
                    case ParserState::AwaitingContent:
                    {
                        if (!precedingOperatorSet && !isFirstEntry)
                        {
                            // Each entry must be preceded by an operator (except the first entry)
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingOperatorInRValueExpression)));
                            break;
                        }

                        // This could be an expression
                        CExpressionParser nestedExpressionParser(this->m_supportsMultiLine, this->m_delimitingEntry);
                        auto parsedNestedExpression = nestedExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedNestedExpression != nullptr)
                        {
                            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedNestedExpression));
                        }
                        else
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisClosure:
                        // Parsing complete
                        parenthesisClose = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;

                    case ParserState::AwaitingContent:
                        // This indicates that we've not been parsing an expression all along
                        return nullptr;

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                }
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnexpectedTerminationOfExpression)));
                localIterator = previousPositionIterator;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Unknown:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::NotPresent:
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'non-present' entry was detected.");

            default:
                throw CMasterParsingException(
                        MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound,
                        "Unrecognized raw-entry type detected.");
        }

        localIterator = ((parserState != ParserState::ParsingComplete) && !doNotAdvance) ?
                        localIterator + 1 :
                        localIterator;

        isFirstEntry = false;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CExpression>(
                new CExpression(parenthesisOpen,
                                parenthesisClose,
                                std::move(parsedContent),
                                std::move(rawEntries),
                                std::move(violations)));
}
