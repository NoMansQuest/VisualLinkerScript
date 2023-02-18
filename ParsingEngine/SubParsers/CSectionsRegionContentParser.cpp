#include <vector>
#include <memory>
#include <string>
#include "CSectionsRegionContentParser.h"
#include "CExpressionParser.h"
#include "Constants.h"
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
        AwaitingAddressOrTypeOrBlockAlignOrColon,
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
 * section [startAddress] [Block(align)] [(type)] :
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
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    auto parserState = ParserState::AwaitingHeader;
    auto doNotAdvance = false;

    CRawEntry sectionOutputNameEntry;
    std::shared_ptr<CExpression> startAddressExpression;
    std::shared_ptr<CExpression> blockAlignExpression;
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


    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto localIteratorPlusOne = localIterator + 1;
        auto localIteratorPlusTwo = localIterator + 2;
        CRawEntry rawEntryPlusOne;
        CRawEntry rawEntryPlusTwo;
        std::string resolvedContentPlusOne;
        std::string resolvedContentPlusTwo;
        if (localIteratorPlusOne != endOfVectorIterator)
        {
            rawEntryPlusOne = *localIteratorPlusOne;
            resolvedContentPlusOne = linkerScriptFile.ResolveRawEntry(rawEntryPlusOne);

            if (localIteratorPlusTwo != endOfVectorIterator)
            {
                rawEntryPlusTwo = *localIteratorPlusTwo;
                resolvedContentPlusTwo = linkerScriptFile.ResolveRawEntry(rawEntryPlusTwo);
            }
        }

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
                parsedContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CComment(std::vector<CRawEntry>{*localIterator}, {})));
                break;
            }

            case RawEntryType::Word:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        // OK, The name should not be reserved keyword
                        sectionOutputNameEntry = *localIterator;
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::SectionOutputNameCannotBeAReservedKeyword));
                        }
                        parserState = ParserState::AwaitingAddressOrTypeOrBlockAlignOrColon;
                        break;
                    }

                    case ParserState::AwaitingAddressOrTypeOrBlockAlignOrColon:
                    {
                        // Here the job is a bit difficult. We need to extract
                        // 1 - [Optional] The 'startAddress' in form of an expression
                        // 2 - [Optional] The 'Block(align)' function call
                        // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                        if (resolvedContent == ":")
                        {
                            // We arrive at the semicolon
                            colonEntry = *localIterator;
                            parserState = ParserState::AwaitingBracketOpen;
                        }
                        else if ((localIterator->EntryType() == RawEntryType::ParenthesisOpen) &&
                                 CParserHelpers::IsSectionOutputType(resolvedContentPlusOne) &&
                                 (rawEntryPlusTwo.EntryType() == RawEntryType::ParenthesisClose))
                        {

                            sectionOutputType = std::shared_ptr<CSectionOutputType>(
                                        new CSectionOutputType(*localIteratorPlusOne,
                                                               *localIterator,
                                                               *localIteratorPlusTwo,
                                                               {*localIteratorPlusOne, *localIterator, *localIteratorPlusTwo},
                                                               {}));
                        }
                        else if ((resolvedContent == "BLOCK") && (rawEntryPlusOne.EntryType() == RawEntryType::ParenthesisOpen))
                        {

                        }
                        else
                        {
                            // Whatever it is, it must be part of the "StartAddress". We need to treat it as an expression.

                        }

                        break;
                    }

                    case ParserState::AwaitingTypeOrColon:
                    {

                        break;
                    }

                    case ParserState::AwaitingColon:
                    {

                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {

                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {

                        break;
                    }

                    case ParserState::AwaitingToRegion:
                    {

                        break;
                    }

                    case ParserState::AwaitingAtLmaRegion:
                    {

                        break;
                    }

                    case ParserState::AwaitingPhdrOrFillExp:
                    {

                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionsRegionContentParser");
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

                break;
            }

            case RawEntryType::BracketOpen:
            {

                break;
            }

            case RawEntryType::BracketClose:
            {

                break;
            }

            case RawEntryType::Unknown:
                throw CMasterParsingException(MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'Unknown' entry was detected.");

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

    return std::shared_ptr<CSectionOutputStatement>(
                new CSectionOutputStatement(sectionOutputNameEntry,
                                            startAddressExpression,
                                            blockAlignExpression,
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
