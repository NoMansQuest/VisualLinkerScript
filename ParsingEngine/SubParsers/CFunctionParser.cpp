#include <vector>
#include <memory>
#include <iterator>

#include "CFunctionParser.h"
#include "CExpressionParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CComment.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/CFunctionCall.h"
#include "../../Models/CParameterSeparator.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;

namespace
{
    /// @brief CFunctionParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };

    enum class ParameterSeparatorState
    {
        AwaitingFirstParameter,
        NowExpectingComma,
        NowExpectingNextParameter,
    };
}

std::shared_ptr<CFunctionCall> CFunctionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    SharedPtrVector<CLinkerScriptContentBase> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    CExpressionParser nestedExpressionParser;

    auto parserState = ParserState::AwaitingName;
    auto parameterSeparatorState = ParameterSeparatorState::AwaitingFirstParameter;

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
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        CRawEntry oneEntryAhead;
        if (localIterator + 1 != endOfVectorIterator)
        {
            oneEntryAhead = *(localIterator+1);
        }

        if (lineChangeDetected)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionsCannotExpandToMultipleLines)));
            break;
        }

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CComment({*localIterator},{})));
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
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionNotRecognized)));
                        }
                        functionNameEntry = *localIterator;
                        parserState = ParserState::AwaitingParenthesisOverture;
                        break;
                    }
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Aborting, this is not a function.
                    }
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        if (parameterSeparatorState == ParameterSeparatorState::NowExpectingComma)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SeparatorExpectedInbetweenParameters)));
                        }

                        // This would be a parameter
                        auto parsedParameter = nestedExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedParameter == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedParameter);
                            parameterSeparatorState = ParameterSeparatorState::NowExpectingComma;
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

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        return nullptr; // this is not a function.
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        parenthesisOverture = *localIterator;
                        parserState = ParserState::AwaitingParenthesisClosure;
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        if (parameterSeparatorState == ParameterSeparatorState::NowExpectingComma)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SeparatorExpectedInbetweenParameters)));
                        }

                        // This would be a parameter
                        auto parsedParameter = nestedExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedParameter == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedParameter);
                            parameterSeparatorState = ParameterSeparatorState::NowExpectingComma;
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

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // This ios not a function.
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        if (parameterSeparatorState == ParameterSeparatorState::AwaitingFirstParameter)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::NoParameterProvidedForTheFunction)));
                        }
                        else if (parameterSeparatorState == ParameterSeparatorState::NowExpectingNextParameter)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingNextParameter)));
                        }

                        parenthesisClosure = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
                break;
            }

            case RawEntryType::Colon:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::QuestionMark:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::ArithmeticOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingFunctioNameHere)));
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingParenthesisOverture)));
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        if (parameterSeparatorState == ParameterSeparatorState::NowExpectingComma)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SeparatorExpectedInbetweenParameters)));
                        }

                        // This would be a parameter
                        auto parsedParameter = nestedExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedParameter == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedParameter);
                            parameterSeparatorState = ParameterSeparatorState::NowExpectingComma;
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

            case RawEntryType::Semicolon:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        localIterator--; // We move iterator one step back, as the semicolon won't be a part raw-entries contained by this element.
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnexpectedTerminationOfFunction)));                        
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
                break;
            }

            case RawEntryType::Comma:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        // We're going for the second parameter, save this and move on
                        if (parameterSeparatorState == ParameterSeparatorState::NowExpectingComma)
                        {
                            parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CParameterSeparator(*localIterator, {*localIterator},{})));
                            parameterSeparatorState = ParameterSeparatorState::NowExpectingNextParameter;
                        }
                        else if (parameterSeparatorState == ParameterSeparatorState::AwaitingFirstParameter)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingFirstParameterHere)));
                        }
                        else if (parameterSeparatorState == ParameterSeparatorState::NowExpectingNextParameter)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingNextParameter)));
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

            case RawEntryType::AssignmentOperator:            
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // This is not a function call
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::AssignmentOperatorNotValidInAnRValueExpression)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CFunctionParser");
                }
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnexpectedTerminationOfExpression)));
                localIterator--;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Unknown:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(
                                            new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
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

        localIterator = (parserState != ParserState::ParsingComplete) ?
                        localIterator + 1 :
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CFunctionCall>(
                new CFunctionCall(functionNameEntry,
                                  parenthesisOverture,
                                  parenthesisClosure,
                                  std::move(parsedContent),
                                  std::move(rawEntries),
                                  std::move(violations)));
}
