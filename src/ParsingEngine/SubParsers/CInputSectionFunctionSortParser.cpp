#include <vector>
#include <memory>
#include <iterator>

#include "ParsingEngine/SubParsers/CInputSectionFunctionSortParser.h"
#include "ParsingEngine/SubParsers/Constants.h"
#include "ParsingEngine/SubParsers/SubParserHelpers.h"

#include "ParsingEngine/CMasterParserException.h"

#include "Models/CComment.h"
#include "Models/Raw/CRawEntry.h"
#include "Models/CWildcardEntry.h"
#include "ParsingEngine/CParserViolation.h"
#include "ParsingEngine/EParserViolationCode.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;

namespace
{
    /// @brief CInputSectionFunctionSortParser parser states
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CFunctionCall> CInputSectionFunctionSortParser::TryParse(
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    LinqVector<CParsedContentBase> parsedContent;
    LinqVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingHeader;
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
        // Edge case coverage, where we read end-of-file prematurely.
        if (localIterator == endOfVectorIterator)
        {
            switch (parserState)
            {
            case ParserState::AwaitingHeader:
            case ParserState::AwaitingParenthesisOverture:
                return nullptr;

            case ParserState::AwaitingParenthesisClosure:
                violations.emplace_back(std::make_shared<CParserViolation>(parenthesisOverture, EParserViolationCode::ParenthesisClosureForExpressionIsMissing));
                --localIterator;
                break;

            default:
                throw CMasterParsingException(
                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                    "ParserState invalid in CInputSectionFunctionSortParser");
            }
        }

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();
        if (lineChangeDetected)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionsCannotSpanMultipleLines)));
            break;
        }

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CParsedContentBase>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        if (CParserHelpers::IsSortFunction(resolvedContent))
                        {
                            functionNameEntry = *localIterator;
                            parserState = ParserState::AwaitingParenthesisOverture;
                        }
                        else
                        {
                            return nullptr;
                        }
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Aborting, this is not a function.
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::ReservedWordsNotAllowedHere)));
                        }
                        else
                        {
                            auto fusedWord = FuseEntriesToFormAWilcardWord(linkerScriptFile, localIterator, endOfVectorIterator);
                            parsedContent.emplace_back(std::shared_ptr<CParsedContentBase>(new CWildcardEntry(fusedWord, { fusedWord }, {})));
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
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
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // This ios not a function.
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        parenthesisClosure = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }

            case RawEntryType::Colon:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::EvaluativeOperators:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }


            case RawEntryType::Wildcard:
            case RawEntryType::QuestionMark:
            case RawEntryType::ArithmeticOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingParenthesisOverture)));
                        break;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        auto fusedWord = FuseEntriesToFormAWilcardWord(linkerScriptFile, localIterator, endOfVectorIterator);
                        parsedContent.emplace_back(std::shared_ptr<CParsedContentBase>(new CWildcardEntry(fusedWord, { fusedWord }, {})));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }

            case RawEntryType::Semicolon:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        localIterator--; // We move iterator one step back, as the semicolon won't be a part raw-entries contained by this element.
                        violations.emplace_back(
                                    std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnexpectedTerminationOfFunction)));
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }

            case RawEntryType::Comma:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(
                                    std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionDoesNotSupportMultipleParameters)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
                }
                break;
            }

            case RawEntryType::AssignmentOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // This is not a function call
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(
                                    std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::AssignmentOperatorNotValidInAnRValueExpression)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionFunctionSortParser");
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

    return std::shared_ptr<CFunctionCall>(
                new CFunctionCall(functionNameEntry,
                                  parenthesisOverture,
                                  parenthesisClosure,
                                  parsedContent,
                                  rawEntries,
                                  violations));
}
