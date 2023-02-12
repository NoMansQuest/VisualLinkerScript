#include "CSectionsRegionContentParser.h"
#include "CExpressionParser.h"
#include <vector>
#include <memory>
#include <string>
#include "../CMasterParserException.h"
#include "../Models/CFunctionCall.h"
#include "../Models/CSectionOutputToRegion.h"
#include "../Models/CSectionOutputAtLmaRegion.h"
#include "../Models/CSectionOutputPhdr.h"
#include "../Models/CSectionOutputFillExpression.h"
#include "../Models/CSectionOutputType.h"
#include "../Models/CSectionOutputStatement.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

namespace
{
    /// @brief Parse states for Sections Region Content Parser
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingAddressOrTypeOrColon,
        AwaitingTypeOrColon,
        AwaitingColon,
        AwaitingBracketOpen,
        AwaitingBracketClosure,
        AwaitingToRegion,
        AwaitingAtLmaRegion,
        AwaitingPhdrOrFillExp,
        ParsingComplete
    };
}

/*
 *
 * section [address] [(type)] :
 *   [AT(lma)]
 *   [ALIGN(section_align) | ALIGN_WITH_INPUT]
 *   [SUBALIGN(subsection_align)]
 *   [constraint]
 *   {
 *     output-section-command
 *     output-section-command
 *     …
 *   } [>region] [AT>lma_region] [:phdr :phdr …] [=fillexp] [,]
 *
 */

std::shared_ptr<CSectionOutputStatement> CSectionsRegionContentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    auto parserState = ParserState::AwaitingHeader;

    CRawEntry sectionOutputNameEntry;
    std::shared_ptr<CExpression> addressExpression;
    std::shared_ptr<CSectionOutputType> sectionOutputType;
    std::shared_ptr<CFunctionCall> atLmaFunction;
    std::shared_ptr<CFunctionCall> alignFunction;
    CRawEntry alignWithInputEntry;
    std::shared_ptr<CFunctionCall> subAlignFunctionCall;
    CRawEntry colonEntry;
    CRawEntry openingBracketEntry;
    CRawEntry closingBracketEntry;
    std::shared_ptr<CSectionOutputToRegion> toVmaRegion;
    std::shared_ptr<CSectionOutputAtLmaRegion> atLmaRegion;
    std::vector<CSectionOutputPhdr> programHeaders;
    std::shared_ptr<CSectionOutputFillExpression> fillExpression;

    /*
    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CComment(std::vector<CRawEntry>{*localIterator}, {})));
                break;
            }

            case RawEntryType::Word:
            {
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    auto parsedStatement = this->m_regionContentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                    if (parsedStatement == nullptr)
                    {
                        CViolation detectedViolation({*localIterator},ViolationCode::EntryInvalidOrMisplaced);
                        violations.emplace_back(detectedViolation);
                    }
                    else
                    {
                        parsedContent.emplace_back(parsedStatement);
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
                    parserState = ParserState::AwaitingOpenningBracket;
                }
                else if (parserState == ParserState::AwaitingOpenningBracket)
                {
                    CViolation detectedViolation({ *localIterator }, ViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader);
                    violations.emplace_back(std::move(detectedViolation));
                }
                break;
            }

            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            {
                if (parserState == ParserState::AwaitingClosingBracket)
                {
                    CViolation detectedViolation({ *localIterator }, ViolationCode::EntryInvalidOrMisplaced);
                }
                else if (parserState == ParserState::AwaitingOpenningBracket)
                {
                    CViolation detectedViolation({ *localIterator }, ViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader);
                    violations.emplace_back(std::move(detectedViolation));
                }
                else if (parserState == ParserState::AwaitingOpenningBracket)
                {
                    parserState = ParserState::ParsingComplete;
                }
                break;
            }

            case RawEntryType::BracketOpen:
            {
                if (parserState == ParserState::AwaitingOpenningBracket)
                {
                    openningBracketEntry = *localIterator;
                    parserState = ParserState::AwaitingClosingBracket;
                }
                else if ((parserState == ParserState::AwaitingClosingBracket) ||
                         (parserState == ParserState::AwaitingHeader))
                {
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
                throw CMasterParsingException(MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'Unknown' entry was detected.");
            }

            case RawEntryType::NotPresent:
            {
                throw CMasterParsingException(MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'non-present' entry was detected.");
            }

            default:
            {
                throw CMasterParsingException(MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound,
                        "Unrecognized raw-entry type detected.");
            }
        }

        localIterator = (parserState != ParserState::ParsingComplete) ?
                        localIterator + 1 :
                        localIterator;
    }*/

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CSectionOutputStatement>(
                new CSectionOutputStatement(sectionOutputNameEntry,
                                            addressExpression,
                                            sectionOutputType,
                                            atLmaFunction,
                                            alignFunction,
                                            alignWithInputEntry,
                                            subAlignFunctionCall,
                                            colonEntry,
                                            openingBracketEntry,
                                            closingBracketEntry,
                                            toVmaRegion,
                                            atLmaRegion,
                                            std::move(programHeaders),
                                            fillExpression,
                                            std::move(parsedContent),
                                            std::move(rawEntries),
                                            std::move(violations)));
}
