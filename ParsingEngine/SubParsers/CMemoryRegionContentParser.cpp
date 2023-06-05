#include "CMemoryRegionContentParser.h"
#include "CAssignmentParser.h"
#include "CMemoryStatementAttributeParser.h"
#include <vector>
#include <memory>
#include <string>
#include "Constants.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../CMasterParserException.h"
#include "../../Models/CComment.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"
#include "../../Models/Raw/CRawFile.h"
#include "../../Models/CMemoryStatement.h"

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

namespace
{
    /// @brief CMemoryRegionContentParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingAttributes,
        AwaitingColon,
        AwaitingOriginAssignment,
        AwaitingLengthAssignment,
        ParsingComplete
    };
}

std::shared_ptr<CMemoryStatement> CMemoryRegionContentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CParserViolation> violations;

    CAssignmentParser assignmentParser;
    CMemoryStatementAttributeParser attributeParser;

    auto parserState = ParserState::AwaitingName;
    auto doNotAdvance = false;

    CRawEntry nameEntry;
    std::shared_ptr<CLinkerScriptContentBase> attributes;
    CRawEntry colonEntry;
    std::shared_ptr<CLinkerScriptContentBase> originAssignment;
    std::shared_ptr<CLinkerScriptContentBase> lengthAssignment;

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
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::MemorySectionNameShouldNotBeAReservedKeyword));
                        }

                        nameEntry = *iterator;
                        parserState = ParserState::AwaitingAttributes;
                        break;
                    }
\
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingAttributes:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    case ParserState::AwaitingOriginAssignment:
                    {
                        originAssignment = assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (originAssignment == nullptr)
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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
                        lengthAssignment = assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (lengthAssignment == nullptr)
                        {
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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
                            attributes = attributeParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (attributes == nullptr)
                            {
                                // Parsing failed, mark this entry as invalid
                                violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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
                            violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
                        }

                        break;
                    }

                    case ParserState::AwaitingName:
                    case ParserState::AwaitingOriginAssignment:
                    case ParserState::AwaitingLengthAssignment:
                    {
                        violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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
            case RawEntryType::ParenthesisClose:
            {
                violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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
                violations.emplace_back(CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced));
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

    if (!nameEntry.IsPresent() || !colonEntry.IsPresent())
    {
        // It is safe to say that we have failed parsing this statement. It's malformed beyond recognition or
        // outright invalid.
        return nullptr;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CMemoryStatement>(
                new CMemoryStatement(nameEntry,
                                     attributes,
                                     colonEntry,
                                     originAssignment,
                                     lengthAssignment,
                                     std::move(rawEntries),
                                     std::move(violations)));
}
