#include "CAssignmentParser.h"
#include "CExpressionParser.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CAssignmentStatement.h"
#include "../Models/CParameterSeparator.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;

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

using namespace VisualLinkerScript::ParsingEngine::SubParsers;

std::shared_ptr<CAssignmentStatement> CAssignmentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    auto parserState = ParserState::AwaitingLValue;
    auto doNotAdvance = false;
    auto isFirstEntry = false;
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
                    case ParserState::AwaitingLValue:
                    {
                        lValueSymbol = *localIterator;
                        parserState = ParserState::AwaitingAssignmentSymbol;
                        break;
                    }

                    case ParserState::AwaitingAssignmentSymbol:
                    {
                        return nullptr; // Abort
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            parsedContent.emplace_back(parsedRValue);
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

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingLValue:
                    case ParserState::AwaitingAssignmentSymbol:
                    case ParserState::AwaitingRValueExpression:
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
                    case ParserState::AwaitingAssignmentSymbol:
                    case ParserState::AwaitingRValueExpression:
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
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::LValueCannotContainRValueExpression));
                            }
                            else
                            {
                                // Abort, we're going up the wrong ladder.
                                return nullptr;
                            }
                        }
                        else
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        if (resolvedContent == ";")
                        {
                            semicolonOperatorEntry = *localIterator;
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
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::MissingLValue));
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
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::MultipleAssignmentOperatorsDetected));
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
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::LValueCannotContainRValueExpression));
                        break;
                    }

                    case ParserState::AwaitingRValueExpression:
                    {
                        parsedRValue = rValueExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedRValue == nullptr)
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            parserState = ParserState::AwaitingSemicolon;
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
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::RValueExpressionParsingFailed));
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

    return std::shared_ptr<CAssignmentStatement>(
                new CAssignmentStatement(lValueSymbol,
                                         assignmentSymbol,
                                         semicolonOperatorEntry,
                                         std::move(parsedContent),
                                         std::move(rawEntries),
                                         std::move(violations)));
}

