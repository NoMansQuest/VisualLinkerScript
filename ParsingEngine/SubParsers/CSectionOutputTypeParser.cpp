#include <vector>
#include <memory>
#include "CSectionOutputTypeParser.h"
#include "../CMasterParserException.h"
#include "Constants.h"
#include "../Models/CSectionOutputType.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CComment.h"
#include "../Models/CMemoryStatementAttribute.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace
{
    /// @brief CPhdrsRegionContentParser parser states
    enum class ParserState
    {
        AwaitingParenthesisOpen,
        AwaitingParenthesisClosure,
        ParsingComplete
    };

}


std::shared_ptr<CMemoryStatementAttribute> CSectionOutputTypeParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    if (iterator->EntryType() != RawEntryType::ParenthesisOpen)
    {
        throw CMasterParsingException(
                MasterParsingExceptionType::InternalParserError,
                "'CMemoryStatementAttributeParser::TryParse' can only be called with 'iterator' pointing to a 'Parenthesis-Open'");
    }

    auto parserState = ParserState::AwaitingParenthesisOpen;
    auto doNotAdvance = false;

    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;
    AttributeDefinitionState readOnlySection;     // Value: R
    AttributeDefinitionState readWriteSection;    // Value: W
    AttributeDefinitionState allocatableSection;  // Value: A
    AttributeDefinitionState executableSection;   // Value: X
    AttributeDefinitionState initializedSection;  // Value: I or L

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        if (lineChangeDetected)
        {
            // Any line-change would be rended parsing attempt null and void (however, it may be possible to report
            // back a type of statement)
            localIterator = previousPositionIterator;
            parserState = ParserState::ParsingComplete;
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
                    case ParserState::AwaitingParenthesisOpen:
                    {
                        return nullptr; // Aborting
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        ParseAttributeValues(
                                    *localIterator,
                                     resolvedContent,
                                     readOnlySection,
                                     readWriteSection,
                                     allocatableSection,
                                     executableSection,
                                     initializedSection,
                                     violations);
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
                    case ParserState::AwaitingParenthesisOpen:
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

    return std::shared_ptr<CMemoryStatementAttribute>(
                new CMemoryStatementAttribute(parenthesisOpen,
                                              parenthesisClose,
                                              readOnlySection,
                                              readWriteSection,
                                              allocatableSection,
                                              executableSection,
                                              initializedSection,
                                              std::move(rawEntries),
                                              std::move(violations)));
}
