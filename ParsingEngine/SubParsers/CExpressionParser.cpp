#include "CExpressionParser.h"
#include <vector>
#include <memory>
#include "Constants.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"
#include "../Models/CComment.h"
#include "../Models/CExpression.h"
#include "../Models/CSymbol.h"
#include "../Models/CExpressionNumber.h"
#include "../Models/CExpressionOperator.h"
#include "../CMasterParserException.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

namespace
{
    /// @brief CExpressionParser parser states
    enum class ParserState
    {
        AwaitingContent,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CLinkerScriptContentBase> CExpressionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    CExpressionParser nestedExpressionParser(ExpressionTerminationType::NoDelimiter, this->m_supportsMultiLine);

    auto parserState = ParserState::AwaitingContent;
    auto doNotAdvance = false;

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
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisClosure:
                    case ParserState::AwaitingContent:
                    {
                        if (CParserHelpers::IsFunctionName(resolvedContent))
                        {
                            if (oneEntryAhead.EntryType() != RawEntryType::ParenthesisOpen)
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::FunctionMissingDefinitionOrInvalidSymbolName));
                                break;
                            }

                            auto parsedFunction = nestedExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedFunction == nullptr)
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                                break;
                            }

                            parsedContent.emplace_back(parsedFunction);
                        }
                        else if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            // We have an invalid statement here
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                            break;
                        }
                        else
                        {
                            CSymbol symbol(*localIterator, {*localIterator}, {});
                            parsedContent.emplace_back(symbol);
                        }

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
            {
                if ((resolvedContent == ";") || (resolvedContent == ","))
                {
                    // Forced completion of parsing
                    localIterator = previousPositionIterator;
                    parserState = ParserState::ParsingComplete;
                }
                else
                {
                    if (CParserHelpers::IsComparisonOperator(resolvedContent) ||
                        CParserHelpers::IsArithmeticOperator(resolvedContent) ||
                        CParserHelpers::IsTernaryOperator(resolvedContent))
                    {
                        CExpressionOperator expressionOperator(*localIterator, {*localIterator}, {});
                        parsedContent.emplace_back(expressionOperator);
                    }
                    else
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::OperatorIsNotAcceptedHere));
                    }
                }
                break;
            }

            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        // This could be an expression
                        auto parsedNestedExpression = expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedNestedExpression->IsValid())
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {

                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                    }
                }
            }

            case RawEntryType::ParenthesisClose:
            {
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                localIterator = previousPositionIterator;
                parserState = ParserState::ParsingComplete;
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

    iterator = localIterator;

    return std::shared_ptr<CExpression>(
                new CExpression(parenthesisOpen,
                                parenthesisClose,
                                std::move(parsedContent),
                                std::move(rawEntries),
                                std::move(violations)));
}
