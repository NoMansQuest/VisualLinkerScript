#include <vector>
#include <memory>
#include <string>

#include "CSectionOutputCommandParser.h"
#include "CExpressionParser.h"
#include "CInputSectionStatementParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CInputSectionStatement.h"
#include "../../Models/CFunctionCall.h"
#include "../../Models/CSectionOutputToVmaRegion.h"
#include "../../Models/CSectionOutputAtLmaRegion.h"
#include "../../Models/CSectionOutputPhdr.h"
#include "../../Models/CSectionOutputFillExpression.h"
#include "../../Models/CSectionOutputCommand.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../../Models/Raw/CRawFile.h"
#include "../../Models/CComment.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

namespace
{
    /// @brief Parse states for Sections Region Content Parser
    enum class ParserState
    {
        AwaitingHeader,                
        AwaitingColon,
        AwaitingBracketOpen,
        AwaitingBracketClosure,
        AwaitingEndOfParse,
        ParsingComplete
    };
}

std::shared_ptr<CSectionOutputCommand> CSectionOutputCommandParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;    
    SharedPtrVector<CViolationBase> violations;

    CExpressionParser expressionParser(ExpressionParserType::NormalParser, false);
    CFunctionParser functionParser;                             // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                         // Example: '. = ALIGN(4);'
    CInputSectionStatementParser inputSectionStatementParser;   // Example: 'foo.o (.input2)'

    auto parserState = ParserState::AwaitingHeader;
    auto doNotAdvance = false;

    CRawEntry sectionOutputNameEntry;
    CRawEntry colonEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> preColonContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> postColonContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> innerContent;
    std::shared_ptr<CSectionOutputToVmaRegion> toVmaRegion;
    std::shared_ptr<CSectionOutputAtLmaRegion> atLmaRegion;
    std::vector<std::shared_ptr<CSectionOutputPhdr>> programHeaders;
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

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                std::shared_ptr<CLinkerScriptContentBase> commentObject(new CComment({*localIterator}, {}));
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingColon:
                    {
                        preColonContent.emplace_back(commentObject);
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        postColonContent.emplace_back(commentObject);
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingEndOfParse:
                    {
                        innerContent.emplace_back(commentObject);
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
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
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword)));
                        }

                        parserState = ParserState::AwaitingColon;
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingColon:
                    {
                        // Here the job is a bit difficult. We need to extract
                        // 1 - [Optional] The 'startAddress' in form of an expression
                        // 2 - [Optional] The 'Block(align)' function call
                        // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                        auto parsedContent =  (localIteratorPlusOne->EntryType() == RawEntryType::ParenthesisOpen) ?
                                              std::dynamic_pointer_cast<CLinkerScriptContentBase>(functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator)) :
                                              std::dynamic_pointer_cast<CLinkerScriptContentBase>(expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));

                        if (parsedContent == nullptr) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            if (parserState == ParserState::AwaitingColon) {
                                preColonContent.emplace_back(parsedContent);
                            } else {
                                postColonContent.emplace_back(parsedContent);
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {                       
                        if (CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent)) {
                            // Such as 'CREATE_OBJECT_SYMBOLS'
                            innerContent.emplace_back();
                        }
                        else if (CParserHelpers::IsOutputSectionDataFunctionName(resolvedContent)) {
                            // Such as "BYTE(1)", "ALIGN(4)", etc.
                            innerContent.emplace_back();
                        }
                        else {
                            // Is this an "Assignment"?
                            auto parsedAssignment = assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedAssignment == nullptr) {
                                // Is this an InputSection definition?
                                auto inputSectionStatement = inputSectionStatementParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (inputSectionStatement == nullptr) {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                } else {
                                    innerContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(inputSectionStatement));
                                }
                            } else {
                                innerContent.emplace_back( std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedAssignment));
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        if ((resolvedContent == "AT") && (resolvedContentPlusOne == ">")){
                            if (rawEntryPlusTwo.IsPresent()){
                                localIterator = localIteratorPlusTwo;
                                atLmaRegion.reset(new CSectionOutputAtLmaRegion(
                                                      *localIterator,
                                                      rawEntryPlusOne,
                                                      rawEntryPlusTwo));

                                localIterator = localIteratorPlusTwo;
                            } else {
                                std::shared_ptr<CViolationBase> missingRegionViolation(new CParserViolation({*localIterator, rawEntryPlusOne}, EParserViolationCode::MissingRegionForAtLmaDefinition));
                                atLmaRegion.reset(new CSectionOutputAtLmaRegion(
                                                      *localIterator,
                                                      rawEntryPlusOne,
                                                      rawEntryPlusTwo,
                                                      { missingRegionViolation }));

                                localIterator = localIteratorPlusOne;
                            }
                        } else {
                            // This is the start of a new expression. We need to rewind the iterator
                            // and return back to the caller
                            localIterator = previousPositionIterator;
                            parserState = ParserState::ParsingComplete;
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }

                break;
            }

            case RawEntryType::Operator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        // Grounds to abort.
                        return nullptr;
                    }

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        if (resolvedContent == ">") {
                            // This could be 'At VMA' definition
                            if (rawEntryPlusOne.EntryType() == RawEntryType::Word) {
                                SharedPtrVector<CViolationBase> toVmaRegionViolations;

                                if (CParserHelpers::IsReservedWord(resolvedContentPlusOne)) {
                                    toVmaRegionViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, rawEntryPlusOne}, EParserViolationCode::VmaRegionNameCannotBeAReservedWord)));
                                }

                                toVmaRegion.reset(new CSectionOutputToVmaRegion(*localIterator,
                                                                                rawEntryPlusOne,
                                                                                {*localIterator, rawEntryPlusOne},
                                                                                std::move(toVmaRegionViolations)));
                                localIterator = localIteratorPlusOne;
                            } else {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                        }
                        else if (resolvedContent == ":")
                        {
                            // This could be 'At VMA' definition
                            if (rawEntryPlusOne.EntryType() == RawEntryType::Word) {
                                SharedPtrVector<CViolationBase> programHeaderViolations;

                                if (CParserHelpers::IsReservedWord(resolvedContentPlusOne)) {
                                    programHeaderViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, rawEntryPlusOne}, EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord)));
                                }

                                auto phdrCall = std::shared_ptr<CSectionOutputPhdr>(
                                        new CSectionOutputPhdr(*localIterator,
                                                               rawEntryPlusOne,
                                                               {*localIterator, rawEntryPlusOne},
                                                               std::move(programHeaderViolations)));

                                programHeaders.emplace_back(phdrCall);
                                localIterator = localIteratorPlusOne;
                            } else {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingProgramHeaderName)));
                            }
                        }
                        else if (resolvedContent == "=")
                        {
                            // This could be 'At VMA' definition
                            if (rawEntryPlusOne.EntryType() == RawEntryType::Number) {
                                fillExpression =  std::shared_ptr<CSectionOutputFillExpression>(
                                            new CSectionOutputFillExpression(*localIterator,
                                                                             rawEntryPlusOne,
                                                                             {*localIterator, rawEntryPlusOne},
                                                                             {}));
                                localIterator = localIteratorPlusOne;
                            } else {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingANumericValueForFillExpression)));
                            }
                        }
                        else
                        {
                            localIterator = previousPositionIterator;
                            parserState = ParserState::ParsingComplete;
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
                break;
            }

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
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        // Grounds to abort.
                        localIterator = previousPositionIterator;
                        parserState = ParserState::ParsingComplete;
                        return nullptr;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        bracketOpenEntry = *localIterator;
                        parserState = ParserState::AwaitingBracketClosure;
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingEndOfParse:
                    {
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionsRegionContentParser");
                }
                break;
            }

            case RawEntryType::BracketClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingBracketOpen:
                    {
                        // Grounds to abort.
                        return nullptr;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        bracketOpenEntry = *localIterator;
                        parserState = ParserState::AwaitingBracketClosure;
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
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

    return std::shared_ptr<CSectionOutputCommand>(
                new CSectionOutputCommand(sectionOutputNameEntry,
                                          std::move(preColonContent),
                                          std::move(postColonContent),
                                          colonEntry,
                                          bracketOpenEntry,
                                          bracketCloseEntry,
                                          toVmaRegion,
                                          atLmaRegion,
                                          std::move(programHeaders),
                                          fillExpression,
                                          std::move(innerContent),
                                          std::move(rawEntries),
                                          std::move(violations)));
}
