#include "CExpressionParser.h"
#include <vector>
#include <memory>
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CUnrecognizableContent.h"
#include "../Models/CViolation.h"
#include "../Models/CExpression.h"
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
    CExpressionParser expressionParser;

    auto parserState = ParserState::AwaitingContent;
    auto doNotAdvance = false;

    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        if (lineChangeDetected)
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
                    case ParserState::AwaitingName:
                    {
                        if (ParserHelpers::IsReservedWord(resolvedContent))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::MemorySectionNameShouldNotBeAReservedKeyword));
                        }

                        nameEntry = *iterator;
                        parserState = ParserState::AwaitingAttributes;
                        break;
                    }
\
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingAttributes:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    case ParserState::AwaitingOriginAssignment:
                    {
                        originAssignment = this->m_assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (originAssignment == nullptr)
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            doNotAdvance = true; // Since another parser was used, the iterator is already correctly positioned
                            parserState = ParserState::AwaitingLengthAssignment;
                        }
                        break;
                    }

                    case ParserState::AwaitingLengthAssignment:
                    {
                        lengthAssignment = this->m_assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (lengthAssignment == nullptr)
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            doNotAdvance = true; // Since another parser was used, the iterator is already correctly positioned
                            parserState = ParserState::ParsingComplete;
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
                switch (parserState)
                {
                    case ParserState::AwaitingAttributes:
                    {
                        if (resolvedContent[0] == '(')
                        {
                            attributes = this->m_attributeParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (attributes == nullptr)
                            {
                                // Parsing failed, mark this entry as invalid
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                                parserState = ParserState::AwaitingAttributes;
                            }
                            else
                            {
                                doNotAdvance = true; // Since another parser was used, the iterator is already correctly positioned
                                parserState = ParserState::AwaitingColon;
                            }
                        }
                        else if (resolvedContent[0] == ':') // This means 'attributes' was not provided (being an 'optional' field)
                        {
                            colonEntry = *localIterator;
                            parserState = ParserState::AwaitingOriginAssignment;
                        }
                        else
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }

                        break;
                    }

                    case ParserState::AwaitingColon:
                    {
                        if (resolvedContent[0] == ':')
                        {
                            colonEntry = *localIterator;
                            parserState = ParserState::AwaitingOriginAssignment;
                        }
                        else
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }

                        break;
                    }

                    case ParserState::AwaitingName:
                    case ParserState::AwaitingOriginAssignment:
                    case ParserState::AwaitingLengthAssignment:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
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
