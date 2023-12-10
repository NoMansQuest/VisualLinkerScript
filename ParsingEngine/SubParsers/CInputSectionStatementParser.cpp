
#include <vector>
#include <memory>
#include <iterator>

#include "CInputSectionStatementParser.h"
#include "CInputSectionFunctionSortParser.h"
#include "CInputSectionFunctionExcludeFileParser.h"

#include "Constants.h"
#include "../CMasterParserException.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/CComment.h"
#include "../../Models/CWildcardEntry.h"
#include "../../Models/CInputSectionStatement.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

namespace
{
    /// @brief CInputSectionParser parser states
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingOptionalParenthesisOverture,
        AwaitingOptionalParenthesisClosure,
        ParsingComplete
    };

    bool IsSpaceBetweenEntries(CRawEntry entry1, CRawEntry entry2)
    {
        if (!entry1.IsPresent() || !entry2.IsPresent())
        {
            return false;
        }

        return entry2.StartPosition() > entry1.StartPosition() + entry1.Length() + 1;
    }
}

std::shared_ptr<CInputSectionStatement> CInputSectionStatementParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    SharedPtrVector<CViolationBase> violations;

    CInputSectionFunctionSortParser sortParser;
    CInputSectionFunctionExcludeFileParser excludeFileParser;

    auto parserState = ParserState::AwaitingHeader;

    std::shared_ptr<CLinkerScriptContentBase> fileSelector;
    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);

        CRawEntry rawEntryPlusOne;
        std::string resolvedContentPlusOne;
        if (localIterator + 1 != endOfVectorIterator)
        {
            rawEntryPlusOne = *(localIterator + 1);
            resolvedContentPlusOne = linkerScriptFile.ResolveRawEntry(rawEntryPlusOne);
        }

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
                    case ParserState::AwaitingHeader:
                    {
                        std::vector<CRawEntry>::const_iterator copyOfIterator = localIterator;
                        auto inputSectionFusedEntry = FuseEntriesToFormAWilcardWord(linkerScriptFile, copyOfIterator, endOfVectorIterator);
                        auto copyOfIteratorPlusOne = copyOfIterator + 1;

                        if (copyOfIteratorPlusOne == endOfVectorIterator) {
                            fileSelector = std::shared_ptr<CLinkerScriptContentBase>(new CWildcardEntry(inputSectionFusedEntry, {inputSectionFusedEntry}, {}));
                            parserState = ParserState::ParsingComplete;
                            localIterator = copyOfIterator; // This is officially our vector.
                        }
                        else
                        {
                            if (copyOfIteratorPlusOne->EntryType() == RawEntryType::ParenthesisOpen)
                            {
                                fileSelector = std::shared_ptr<CLinkerScriptContentBase>(new CWildcardEntry(inputSectionFusedEntry, {inputSectionFusedEntry}, {}));
                                localIterator = copyOfIterator; // This is officially our vector.
                                parserState = ParserState::AwaitingOptionalParenthesisOverture;
                            }
                            else if (copyOfIteratorPlusOne->EntryType() != RawEntryType::ParenthesisClose && // Check for false positive.
                                copyOfIteratorPlusOne->EntryType() != RawEntryType::BracketClose &&
                                copyOfIteratorPlusOne->EntryType() != RawEntryType::Word &&
                                copyOfIteratorPlusOne->EntryType() != RawEntryType::Comment &&
                                !IsSpaceBetweenEntries(*copyOfIterator, *copyOfIteratorPlusOne))
                            {
                                return nullptr; // This could be some weird jibber jabber (i.e. *.text.co#something). The #somethign part invalidates the wildcard word.
                            }
                            else
                            {
                                fileSelector = std::shared_ptr<CLinkerScriptContentBase>(new CWildcardEntry(inputSectionFusedEntry, {inputSectionFusedEntry}, {}));
                                parserState = ParserState::ParsingComplete;
                                localIterator = copyOfIterator; // This is officially our vector.
                            }
                        }

                        break;
                    }
                    case ParserState::AwaitingOptionalParenthesisOverture:
                    {
                        // This must be the next entry. We need to abort (successfully)
                        localIterator--;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    case ParserState::AwaitingOptionalParenthesisClosure:
                    {
                        auto fusedWord = FuseEntriesToFormAWilcardWord(linkerScriptFile, localIterator, endOfVectorIterator);
                        parsedContent.emplace_back(CWildcardEntry(fusedWord, { fusedWord }, {}));
                        parserState = ParserState::AwaitingOptionalParenthesisClosure;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CInputSectionStatementParser.");
                }
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
                            if (rawEntryPlusOne.EntryType() == RawEntryType::ParenthesisOpen)
                            {
                                // This is a SORT function.
                                auto parsedSortFunctionCall = sortParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                fileSelector = std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedSortFunctionCall);
                                parserState = ParserState::AwaitingOptionalParenthesisOverture;
                            }
                            else
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MissingFunctionBody)));
                            }
                        }
                        else
                        {
                            if (CParserHelpers::IsReservedWord(resolvedContent))
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                            else
                            {
                                auto headerFusedEntry = FuseEntriesToFormAWilcardWord(linkerScriptFile, localIterator, endOfVectorIterator);
                                fileSelector = std::shared_ptr<CLinkerScriptContentBase>(new CWildcardEntry(headerFusedEntry, {headerFusedEntry}, {}));
                                if ((localIterator + 1 != endOfVectorIterator) && ((localIterator+1)->EntryType() == RawEntryType::ParenthesisOpen))
                                {
                                    parserState = ParserState::AwaitingOptionalParenthesisOverture;
                                }
                                else
                                {
                                    parserState = ParserState::ParsingComplete;
                                }
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingOptionalParenthesisOverture:
                    {
                        // NOTE: This is a deal breaker, as in this particular parser we << only transition to this state >> when we know for a fact
                        // that this parenthesis-overture is where it's expected to be.
                        return nullptr;
                    }

                    case ParserState::AwaitingOptionalParenthesisClosure:
                    {
                        auto nextEntryIterator = FindNextNonCommentEntry(linkerScriptFile, localIterator, endOfVectorIterator);
                        auto nextNonCommentRawEntry = (nextEntryIterator != endOfVectorIterator) ? *nextEntryIterator : CRawEntry();

                        if (nextNonCommentRawEntry.IsPresent() && nextNonCommentRawEntry.EntryType() == RawEntryType::ParenthesisOpen)
                        {
                            if (CParserHelpers::IsSortFunction(resolvedContent))
                            {
                                auto parsedSortFunctionCall = sortParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedSortFunctionCall == nullptr)
                                {
                                    // Although theoretically this should never happen, as we already know we're dealing with a SORT function call.
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::InvalidSortFunctionCall)));
                                }
                                else
                                {
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedSortFunctionCall));
                                }
                            }
                            else if (CParserHelpers::IsExcludeFileCall(resolvedContent))
                            {
                                auto parsedExcludeFileCall = excludeFileParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedExcludeFileCall == nullptr)
                                {
                                    // Although theoretically this should never happen, as we already know we're dealing with a SORT function call.
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::InvalidExcludeFileCall)));
                                }
                                else
                                {
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedExcludeFileCall));
                                }
                            }
                            else
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::IllegalFunctionCallInSectionOutputStatement)));
                            }
                        }
                        else if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            // This is a normal section definition. We accept it.
                            auto fusedEntry = FuseEntriesToFormAWilcardWord(linkerScriptFile, localIterator, endOfVectorIterator);
                            parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CWildcardEntry(fusedEntry, {fusedEntry}, {})));
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryStatementAttributeParser");
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingOptionalParenthesisOverture:
                    {
                        parenthesisOpen = *localIterator;
                        parserState = ParserState::AwaitingOptionalParenthesisClosure;
                        break;
                    }
                    default:
                    {
                        // NOTE: This is a deal breaker, as in this particular parser we only transition to this state when we know for a fact
                        // that this parenthesis-overture is where it's expected to be.
                        return nullptr;
                    }
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingOptionalParenthesisClosure:
                    {
                        parenthesisClose = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }
                    default:
                    {
                        // NOTE: This is a deal breaker, we best abort.
                        return nullptr;
                    }
                }
                break;
            }

            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                // This needs to be handled by the CMemoryRegionParser, and not here...
                localIterator--;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Comma:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::Semicolon:
            case RawEntryType::QuestionMark:
            case RawEntryType::Colon:
            case RawEntryType::ArithmeticOperator:
            {
                // We ignore commas
                if ((parserState != ParserState::AwaitingOptionalParenthesisClosure) || (resolvedContent != ","))
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                }
                break;
            }

            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:
            case RawEntryType::String:
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

    return std::shared_ptr<CInputSectionStatement>(
                new CInputSectionStatement(fileSelector,
                                           parenthesisOpen,
                                           parenthesisClose,
                                           std::move(parsedContent),
                                           std::move(rawEntries),
                                           std::move(violations)));
}
