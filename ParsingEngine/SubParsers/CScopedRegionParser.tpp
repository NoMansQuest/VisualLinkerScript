#include "CScopedRegionParser.h"

#include "../CMasterParserException.h"

#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/CRawFile.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../../Models/CComment.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"


using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

namespace
{
    /// @brief Parse states for Memory-Region
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingOpeningBracket,
        AwaitingClosingBracket,
        ParsingComplete
    };
}

template <SubParserType TParserType, typename TContentParserType, class TProducingOutputType>
std::shared_ptr<TProducingOutputType> CScopedRegionParser<TParserType, TContentParserType, TProducingOutputType>::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    auto parserState = ParserState::AwaitingHeader;

    TContentParserType regionContentParser;

    CRawEntry regionHeaderEntry;
    CRawEntry openingBracketEntry;
    CRawEntry closingBracketEntry;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CComment(std::vector<CRawEntry>{*localIterator}, {})));
                break;
            }

            case RawEntryType::Wildcard:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingOpeningBracket:
                    case ParserState::AwaitingClosingBracket:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CScopedRegionParser.");
                }
                break;
            }

            case RawEntryType::Word:
            {
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    auto parsedStatement = regionContentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                    if (parsedStatement == nullptr)
                    {                        
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator},EParserViolationCode::EntryInvalidOrMisplaced)));
                    }
                    else
                    {
                        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedStatement));
                    }
                }
                else if (parserState == ParserState::AwaitingHeader)
                {
                    auto stringContent = linkerScriptFile.ResolveRawEntry(*localIterator);
                    if (stringContent != this->GetHeaderName())
                    {
                        // Full abort in this case
                        return nullptr;
                    }

                    regionHeaderEntry = *localIterator;
                    parserState = ParserState::AwaitingOpeningBracket;
                }
                else if (parserState == ParserState::AwaitingOpeningBracket)
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({ *localIterator }, EParserViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader)));
                }
                break;
            }

            case RawEntryType::QuestionMark:
            case RawEntryType::ArithmeticOperator:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            {
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({ *localIterator }, EParserViolationCode::EntryInvalidOrMisplaced)));
                }
                else if (parserState == ParserState::AwaitingOpeningBracket)
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({ *localIterator }, EParserViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader)));
                }
                else if (parserState == ParserState::AwaitingOpeningBracket)
                {
                    localIterator--;
                    parserState = ParserState::ParsingComplete;
                }
                break;
            }

            case RawEntryType::BracketOpen:
            {
                if (parserState == ParserState::AwaitingOpeningBracket)
                {
                    openingBracketEntry = *localIterator;
                    parserState = ParserState::AwaitingClosingBracket;
                }
                else if ((parserState == ParserState::AwaitingClosingBracket) ||
                         (parserState == ParserState::AwaitingHeader))
                {
                    localIterator--; // This entry here has to be parsed by someone else.
                    parserState = ParserState::ParsingComplete;
                }
                break;
            }

            case RawEntryType::BracketClose:
            {
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    closingBracketEntry = *localIterator;
                    parserState = ParserState::ParsingComplete;
                }
                break;
            }

            case RawEntryType::Unknown:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingOpeningBracket:
                    case ParserState::AwaitingClosingBracket:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CScopedRegionParser.");
                }
                break;
            }

            case RawEntryType::NotPresent:
                throw CMasterParsingException(MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'non-present' entry was detected.");

            default:
                throw CMasterParsingException(MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound,
                        "Unrecognized raw-entry type detected.");
        }

        localIterator = (parserState != ParserState::ParsingComplete) ?
                        localIterator + 1 :
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<TProducingOutputType>(
                new TProducingOutputType(regionHeaderEntry,
                                         openingBracketEntry,
                                         closingBracketEntry,
                                         parsedContent,
                                         rawEntries,
                                         violations));
}
