#include <vector>
#include <memory>
#include <iterator>

#include "CAssignmentProcedureParser.h"
#include "CAssignmentParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CComment.h"
#include "../../Models/CAssignmentStatement.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/CAssignmentProcedureStatement.h"
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
        AwaitingProcedureName,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        AwaitingSemicolon,
        ParsingComplete
    };
}

std::shared_ptr<CAssignmentProcedureStatement> CAssignmentProcedureParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    auto parserState = ParserState::AwaitingProcedureName;

    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    CRawEntry proceduerNameEntry;
    CRawEntry parenthesisOpenEntry;
    CRawEntry parenthesisCloseEntry;
    CRawEntry lValueSymbol;    
    CRawEntry semicolonOperatorEntry;
    std::shared_ptr<CAssignmentStatement> assignmentStatement;

    CAssignmentParser assignmentParser(true);

    if (localIterator->EntryType() != RawEntryType::Word)
    {
        // Parsing failed...
        return nullptr;
    }

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        auto localIteratorPlusOne = localIterator + 1;
        auto nextEntryExists = (localIteratorPlusOne != endOfVectorIterator);

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto resolvedContentPlusOne = nextEntryExists ? linkerScriptFile.ResolveRawEntry(*localIteratorPlusOne) : "";

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
                    case ParserState::AwaitingProcedureName:
                    {
                        if (!CParserHelpers::IsAssignmentProcedure(resolvedContent))
                        {
                            return nullptr;
                        }

                        proceduerNameEntry = *localIterator;
                        parserState = ParserState::AwaitingParenthesisOverture;
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }


                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingSemicolonHere)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentProcedureParser");
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingProcedureName:
                    {
                        return nullptr; // Grounds to abort.
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        parenthesisOpenEntry = *localIterator;

                        if (!nextEntryExists)
                        {
                            parserState = ParserState::ParsingComplete;
                            break;
                        }

                        // We continue by simply parsing the content for assignment statement.
                        // Note: In case we fail parsing interio statement, the 'assignmentStatement' will be set to nullptr
                        //       and we'll contiue looking for 'parenthesisClosure'. Anything found inbetween will be automatically
                        //       marked as 'misplaced'.
                        localIterator = localIteratorPlusOne;
                        assignmentStatement = assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        parserState = ParserState::AwaitingParenthesisClosure;
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
                                    "ParserState invalid in CAssignmentProcedureParser");
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingProcedureName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Grounds to abort.
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        parenthesisCloseEntry = *localIterator;
                        parserState = ParserState::AwaitingSemicolon;
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
                                    "ParserState invalid in CAssignmentProcedureParser");
                }
                break;
            }

            case RawEntryType::Colon:
            case RawEntryType::Semicolon:
            case RawEntryType::Comma:
            case RawEntryType::QuestionMark:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::ArithmeticOperator:
            case RawEntryType::AssignmentOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingProcedureName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Grounds to abort.
                    }

                    case ParserState::AwaitingSemicolon:
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentProcedureParser");
                }
            }

            case RawEntryType::Number:
            case RawEntryType::String:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingProcedureName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Grounds to abort.
                    }

                    case ParserState::AwaitingSemicolon:
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentProcedureParser");
                }
                break;
            }

            // Brackets are sensitive and the matter needs to be escalated to higher-up
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingProcedureName:
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Grounds to abort.
                    }

                    case ParserState::AwaitingSemicolon:
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        localIterator--;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CAssignmentProcedureParser");
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
    std::copy(parsingStartIteratorPosition, localIterator, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CAssignmentProcedureStatement>(
                new CAssignmentProcedureStatement(proceduerNameEntry,
                                                  parenthesisOpenEntry,
                                                  parenthesisCloseEntry,
                                                  assignmentStatement,
                                                  semicolonOperatorEntry,
                                                  std::move(parsedContent),
                                                  std::move(rawEntries),
                                                  std::move(violations)));
}
