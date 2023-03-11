#include <vector>
#include <memory>
#include "CInputSectionFunctionParser.h"
#include "Constants.h"
#include "../CMasterParserException.h"
#include "../Models/CInputSectionTargetSection.h"
#include "../Models/CComment.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CInputSectionFunction.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

namespace
{
    /// @brief CInputSectionFunctionParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CInputSectionFunction> CInputSectionFunctionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    CInputSectionFunctionParser nestedFunctionParser;

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
        std::string oneEntryAheadResolved;
        if (localIterator + 1 != endOfVectorIterator)
        {
            oneEntryAhead = *(localIterator+1);
            oneEntryAheadResolved = linkerScriptFile.ResolveRawEntry(oneEntryAhead);
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
                        if (!CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent))
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

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        // Ok, this could either be a function or a object-file input-section
                        if (!CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent))
                        {
                            if (oneEntryAhead.IsPresent() && oneEntryAhead.EntryType() == RawEntryType::ParenthesisOpen )
                            {
                                auto parsedInnerFunction = nestedFunctionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedInnerFunction == nullptr)
                                {
                                    violations.emplace_back(CViolation(*localIterator, ViolationCode::InputSectionFunctionParsingFailed));
                                }
                                else
                                {
                                    parsedContent.emplace_back(parsedInnerFunction);
                                }
                            }
                            else
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::MissingParenthesisOvertureForFunction));
                            }
                        }
                        else
                        {
                            auto targetInputSection = std::shared_ptr<CLinkerScriptContentBase>(new CInputSectionTargetSection(*localIterator, {}));
                            parsedContent.emplace_back(targetInputSection);
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
                                    "ParserState invalid in CInputSectionFunctionParser");
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
                                    "ParserState invalid in CInputSectionFunctionParser");
                }
            }

            case RawEntryType::Operator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingName:
                    case ParserState::AwaitingParenthesisOverture:
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::OperatorsNotAllowedWithinInputSectionFunctions));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionParser");
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
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
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

        isFirstEntry = false;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CInputSectionFunction>(
                new CInputSectionFunction(functionNameEntry,
                                          parenthesisOverture,
                                          parenthesisClosure,
                                          std::move(parsedContent),
                                          std::move(rawEntries),
                                          std::move(violations)));
}