
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
        AwaitingOperator,
        ParsingComplete
    };

    /// @brief CExpressionParser ternary operation states
    enum class TernaryState
    {        
        AwaitingThenExpression,
        AwaitingElseExpression,
    };

    bool CanOperatorsCoexist(CRawEntry leftOperator, CRawEntry rightOperator, CRawFile& linkerScriptFile)
    {
        auto op2Resolved = linkerScriptFile.ResolveRawEntry(rightOperator);
        if (rightOperator.EntryType() == RawEntryType::ArithmeticOperator)
        {
            return (StringEquals(op2Resolved, "-") || StringEquals(op2Resolved, "+"));
        }

        return false;
    }
}

std::shared_ptr<CExpression> CExpressionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingContent;
    auto ternaryState = TernaryState::AwaitingThenExpression;

    CRawEntry previousOperator;
    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;

    // Capture parenthesis overture if present. This will then have to be matched with a parenthesis closure.
    if (this->m_isParenthesizedExpressionParser &&
        (localIterator != endOfVectorIterator) &&
        (*localIterator).EntryType() == RawEntryType::ParenthesisOpen)
    {
        parenthesisOpen = *localIterator++; // We both save this parenthesis and increment the iterator.
    }

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();
        auto oneEntryAhead = (localIterator + 1 != endOfVectorIterator) ? *(localIterator+1) : CRawEntry();

        if (lineChangeDetected && !this->m_supportsMultiLine)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnexpectedTerminationOfExpression)));
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
                    case ParserState::AwaitingOperator:
                    {
                        // Each entry must be preceded by an operator (except the first entry)
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingOperatorInRValueExpression)));
                        break;
                    }

                    case ParserState::AwaitingContent:
                    {
                        if (CParserHelpers::IsFunctionName(resolvedContent))
                        {
                            CFunctionParser functionParser(true, true);
                            auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedFunction == nullptr)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));                                
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedFunction));
                                parserState = ParserState::AwaitingOperator; // Parser state switches here.
                            }
                        }
                        else if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));                            
                        }
                        else
                        {
                            auto symbol = std::shared_ptr<CLinkerScriptContentBase>(new CSymbol(*localIterator, {*localIterator}, {}));
                            parsedContent.emplace_back(symbol);
                            parserState = ParserState::AwaitingOperator; // Parser state switches here.
                        }

                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CExpressionParser");
                }                
                break;
            }

            case RawEntryType::Semicolon:
            case RawEntryType::Comma:
            {
                if (parserState == ParserState::AwaitingContent)
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingASymbolOrNumberBeforeExpressionEnds)));
                }

                if (parenthesisOpen.IsPresent())
                {
                    // We need to complain as the 'parenthesisClosure' was not found
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingParenthesisClosure)));
                }

                // Forced completion of parsing
                localIterator--;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Colon:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        // An exceptional authorized case is when signed operators are found right after evaluative operators.
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingASymbolOrNumberHere)));

                        switch (ternaryState)
                        {
                            case TernaryState::AwaitingThenExpression:
                            {
                                // EXCPETION: When parsing 'address' of a section-output command, an unexpected
                                // colon spells the end of parsing, pretty much like semicolon.
                                if (this->m_isSectionOutputAddressParser)
                                {
                                    localIterator--;
                                    parserState = ParserState::ParsingComplete;
                                }
                                else
                                {
                                    // Ternary wise it is NOT OK.
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::TernaryExpressionMissingElseOutcome)));
                                }
                                break;
                            }

                            case TernaryState::AwaitingElseExpression:
                            {
                                // Ternary wise it's OK
                                auto ternaryIf = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                                parsedContent.emplace_back(ternaryIf);
                                ternaryState = TernaryState::AwaitingThenExpression;
                                break;
                            }

                            default:
                                throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "TernaryState invalid in CExpressionParser");
                        }
                        // Note: parserState remains at 'AwaitingContent'.
                        break;
                    }

                    case ParserState::AwaitingOperator:
                    {
                        switch (ternaryState)
                        {
                            case TernaryState::AwaitingThenExpression:
                            {
                                // EXCPETION: When parsing 'address' of a section-output command, an unexpected
                                // colon spells the end of parsing, pretty much like semicolon.
                                if (this->m_isSectionOutputAddressParser)
                                {
                                    localIterator--;
                                    parserState = ParserState::ParsingComplete;
                                }
                                else
                                {
                                    // Ternary wise it is NOT OK.
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::TernaryExpressionMissingElseOutcome)));
                                }
                            }

                            case TernaryState::AwaitingElseExpression:
                            {
                                // Ternary wise it's OK
                                auto ternaryIf = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                                parsedContent.emplace_back(ternaryIf);
                                ternaryState = TernaryState::AwaitingThenExpression;
                                break;
                            }

                            default:
                                throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "TernaryState invalid in CExpressionParser");
                        }
                        parserState = ParserState::AwaitingContent;
                        break;
                    }

                    default:
                        throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                            "ParserState invalid in CExpressionParser");
                }

                previousOperator = CRawEntry();
                break;
            }

            case RawEntryType::QuestionMark:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        // An exceptional authorized case is when signed operators are found right after evaluative operators.
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingASymbolOrNumberHere)));

                        switch (ternaryState)
                        {
                            case TernaryState::AwaitingThenExpression:
                            {
                                // Ternary wise it's OK
                                auto ternaryIf = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                                parsedContent.emplace_back(ternaryIf);
                                ternaryState = TernaryState::AwaitingElseExpression;
                                break;
                            }

                            case TernaryState::AwaitingElseExpression:
                            {
                                // Ternary wise it is NOT OK.
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::TernaryExpressionMissingFirstCondition)));
                                break;
                            }

                            default:
                                throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "TernaryState invalid in CExpressionParser");
                        }
                        // Note: parserState remains at 'AwaitingContent'.
                        break;
                    }

                    case ParserState::AwaitingOperator:
                    {
                        switch (ternaryState)
                        {
                            case TernaryState::AwaitingThenExpression:
                            {
                                // Ternary wise it's OK
                                auto ternaryIf = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                                parsedContent.emplace_back(ternaryIf);
                                ternaryState = TernaryState::AwaitingElseExpression;
                                break;
                            }

                            case TernaryState::AwaitingElseExpression:
                            {
                                // Ternary wise it is NOT OK.
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::TernaryExpressionMissingFirstCondition)));
                                break;
                            }

                            default:
                                throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "TernaryState invalid in CExpressionParser");
                        }
                        parserState = ParserState::AwaitingContent;
                        break;
                    }

                    default:
                        throw CMasterParsingException(MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                            "ParserState invalid in CExpressionParser");
                }

                previousOperator = CRawEntry();
                break;
            }

            case RawEntryType::EvaluativeOperators:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        // An exceptional authorized case is when signed operators are found right after evaluative operators.
                        if (CanOperatorsCoexist(*localIterator, previousOperator, linkerScriptFile))
                        {
                            auto genericOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionNumber(*localIterator, {*localIterator}, {}));
                            parsedContent.emplace_back(genericOperator);
                            break;
                        }

                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingASymbolOrNumberHere)));
                        auto evaluativeOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(evaluativeOperator);
                        break;
                    }

                    case ParserState::AwaitingOperator:
                    {
                        auto genericOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionNumber(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(genericOperator);
                        parserState = ParserState::AwaitingContent;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CExpressionParser");
                }

                previousOperator = *localIterator;
                break;
            }

            case RawEntryType::ArithmeticOperator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        // An exceptional authorized case is when signed operators are put consecutively.
                        if (CanOperatorsCoexist(*localIterator, previousOperator, linkerScriptFile))
                        {
                            auto genericOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionNumber(*localIterator, {*localIterator}, {}));
                            parsedContent.emplace_back(genericOperator);
                            break;
                        }

                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingASymbolOrNumberHere)));
                        auto genericOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(genericOperator);
                        break;
                    }

                    case ParserState::AwaitingOperator:
                    {
                        auto genericOperator = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionOperator(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(genericOperator);
                        parserState = ParserState::AwaitingContent;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CExpressionParser");
                }

                previousOperator = *localIterator;
                break;
            }

            case RawEntryType::AssignmentOperator:
            {
                // This indicates that we've not been parsing an expression all along
                return nullptr;
            }

            case RawEntryType::Number:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        auto numericValue = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionNumber(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(numericValue);
                        parserState = ParserState::AwaitingOperator;
                        break;
                    }

                    case ParserState::AwaitingOperator:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingAnOperatorHere)));
                        auto numericValue = std::shared_ptr<CLinkerScriptContentBase>(new CExpressionNumber(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(numericValue);
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CExpressionParser");
                }

                previousOperator = CRawEntry();
                break;
            }

            case RawEntryType::String:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    {
                        auto stringEntry = std::shared_ptr<CLinkerScriptContentBase>(new CStringEntry(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(stringEntry);
                        parserState = ParserState::AwaitingOperator;
                        break;
                    }

                    case ParserState::AwaitingOperator:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingAnOperatorHere)));
                        auto stringEntry = std::shared_ptr<CLinkerScriptContentBase>(new CStringEntry(*localIterator, {*localIterator}, {}));
                        parsedContent.emplace_back(stringEntry);
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CExpressionParser");
                }

                previousOperator = CRawEntry();
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingOperator:
                    case ParserState::AwaitingContent:
                    {
                        if (parserState == ParserState::AwaitingOperator)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingAnOperatorHere)));
                        }

                        // This could be an expression
                        CExpressionParser nestedExpressionParser(this->m_supportsMultiLine, true, false);
                        auto parsedNestedExpression = nestedExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedNestedExpression != nullptr)
                        {
                            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedNestedExpression));
                            parserState = ParserState::AwaitingOperator;
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
                                    "ParserState invalid in CExpressionParser");
                }

                previousOperator = CRawEntry();
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingContent:
                    case ParserState::AwaitingOperator:
                    {
                        if (parenthesisOpen.IsPresent())
                        {
                            parenthesisClose = *localIterator;
                        }
                        else
                        {
                            localIterator--;
                        }

                        if (parserState == ParserState::AwaitingContent)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::OperatorIsNotAcceptedHere)));
                        }

                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CExpressionParser");
                }

                previousOperator = CRawEntry();
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
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'non-present' entry was detected.");

            default:
                throw CMasterParsingException(
                        MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound,
                        "Unrecognized raw-entry type detected.");
        }

        localIterator += ((parserState != ParserState::ParsingComplete) ? 1 : 0);
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
