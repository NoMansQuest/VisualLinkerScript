
#include <vector>
#include <memory>
#include "CInputSectionStatementParser.h"
#include "CInputSectionFunctionParser.h"
#include "Constants.h"
#include "../CMasterParserException.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CComment.h"
#include "../Models/CInputSectionStatement.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace
{
    /// @brief CInputSectionParser parser states
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };
}

std::shared_ptr<CInputSectionStatement> CInputSectionStatementParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<CViolation> violations;

    CInputSectionFunctionParser inputSectionFunctionParser;

    auto parserState = ParserState::AwaitingHeader;
    auto doNotAdvance = false;

    CRawEntry inputSectionHeader;
    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
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

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            return nullptr; // This is not an input-section
                        }

                        inputSectionHeader = *localIterator;
                        parserState = (resolvedContentPlusOne == "(") ?
                                      ParserState::AwaitingParenthesisOverture :
                                      ParserState::ParsingComplete;
                        break;
                    }

                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // This is a deal breaker
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            if (CParserHelpers::IsInputSectionFunction(resolvedContent))
                            {
                                auto parsedInputSectionFunction = inputSectionFunctionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedInputSectionFunction == nullptr)
                                {
                                    violations.emplace_back(CViolation(*localIterator, ViolationCode::InvalidSyntaxInSectionInputFunctionDeclaration));
                                }
                                else
                                {
                                    parsedContent.emplace_back(parsedInputSectionFunction);
                                }
                            }
                            else
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryNotAllowedInInputSectionDefinition));
                            }
                        }
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryStatementAttributeParser");
                    }
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        parenthesisOpen = *localIterator;
                        parserState = ParserState::AwaitingParenthesisClosure;
                        break;
                    }
                    default:
                    {
                        return nullptr; // Parsing failed
                    }
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        parenthesisClose = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }
                    default:
                    {
                        return nullptr; // Parsing failed
                    }
                }
                break;
            }

            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                // This needs to be handled by the CMemoryRegionParser, and not here...
                localIterator = previousPositionIterator;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
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
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CInputSectionStatement>(
                new CInputSectionStatement(inputSectionHeader,
                                           parenthesisOpen,
                                           parenthesisClose,
                                           std::move(parsedContent),
                                           std::move(rawEntries),
                                           std::move(violations)));
}
