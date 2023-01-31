#include "CFunctionParser.h"
#include "CExpressionParser.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CFunctionCall.h"
#include "../Models/CParameterSeparator.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

namespace
{
    /// @brief CMemoryRegionContentParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CFunctionCall> CFunctionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    CExpressionParser nestedExpressionParser(ExpressionParserType::NormalParser, false);

    auto parserState = ParserState::AwaitingName;
    auto doNotAdvance = false;
    auto isFirstEntry = false;

    CRawEntry functionNameEntry;
    CRawEntry parenthesisOverture;
    CRawEntry parenthesisClosure;

    if (localIterator->EntryType() != RawEntryType::Word)
    {
        // Parsing failed...
        return nullptr;
    }

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

        if (lineChangeDetected)
        {
            violations.emplace_back(CViolation(*localIterator, ViolationCode::FunctionsCannotExpandToMultipleLines));
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
                    case ParserState::AwaitingName:
                    {
                        if (!CParserHelpers::IsFunctionName(resolvedContent))
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::FunctionNotRecognized));
                        }
                        functionNameEntry = *localIterator;
                        break;
                    }
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingFunctioNameHere));
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        parenthesisOverture = *localIterator;
                        parserState = ParserState::AwaitingParenthesisClosure;
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingAnotherParameterOrParenthesisClosure));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingFunctioNameHere));
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingParenthesisOverture));
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        // This could be an expression
                        parenthesisClosure = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
            }

            case RawEntryType::Operator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingFunctioNameHere));
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingParenthesisOverture));
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        // This could be an expression
                        if (resolvedContent == ",")
                        {
                            // We're going for the second parameter, save this and move on
                            parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(
                                                           new CParameterSeparator(*localIterator, {*localIterator},{})));

                        }
                        else if (resolvedContent == ";")
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::UnexpectedTerminationOfFunction));
                            parserState = ParserState::ParsingComplete;
                        }
                        else
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
                break;
            }

            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            {
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                violations.emplace_back(CViolation(*localIterator, ViolationCode::UnexpectedTerminationOfExpression));
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

        isFirstEntry = false;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CFunctionCall>(
                new CFunctionCall(functionNameEntry,
                                  parenthesisOverture,
                                  parenthesisClosure,
                                  std::move(parsedContent),
                                  std::move(rawEntries),
                                  std::move(violations)));
}
