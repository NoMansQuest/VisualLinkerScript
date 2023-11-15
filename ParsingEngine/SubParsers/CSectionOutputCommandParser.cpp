#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CSectionOutputCommandParser.h"
#include "CExpressionParser.h"
#include "CInputSectionStatementParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CInputSectionStatement.h"
#include "../../Models/CFunctionCall.h"
#include "../../Models/CSectionOutputDataExpression.h"
#include "../../Models/CSectionOutputToVmaRegion.h"
#include "../../Models/CSectionOutputAtLmaRegion.h"
#include "../../Models/CSectionOutputPhdr.h"
#include "../../Models/CSectionOutputFillExpression.h"
#include "../../Models/CSectionOutputCommand.h"
#include "../../Models/CSectionOutputType.h"
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
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;    
    SharedPtrVector<CViolationBase> violations;

    CExpressionParser expressionParser;
    CFunctionParser functionParser;                             // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                         // Example: '. = ALIGN(4);'
    CInputSectionStatementParser inputSectionStatementParser;   // Example: 'foo.o (.input2)'

    auto parserState = ParserState::AwaitingHeader;

    CRawEntry sectionOutputNameEntry;
    CRawEntry colonEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> preColonContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> postColonContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> innerContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> endingContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);

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
                        auto parsedContent =  std::dynamic_pointer_cast<CLinkerScriptContentBase>(expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
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
                            innerContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputDataExpression(*localIterator)));

                        } else if (CParserHelpers::IsOutputSectionDataFunctionName(resolvedContent)) {
                            // Such as "BYTE(1)", "ALIGN(4)", etc.
                            auto parsedFunctionCall = functionParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                            if (parsedFunctionCall == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedFunctionCall));
                            }

                        } else {
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
                        auto matchResultForAtRegion = MatchSequenceOpenEnded(linkerScriptFile, iterator, endOfVectorIterator, {"AT", ">"});
                        if (matchResultForAtRegion.Successful())
                        {
                            auto potentialRegionName = FindNextNonCommentEntry(linkerScriptFile, iterator + 2, endOfVectorIterator);
                            if (potentialRegionName == endOfVectorIterator)
                            {
                                std::shared_ptr<CViolationBase> missingRegionViolation(new CParserViolation({*localIterator, matchResultForAtRegion.MatchedElements()[1]}, EParserViolationCode::MissingRegionForAtLmaDefinition));
                                auto atLmaRegion = std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputAtLmaRegion(
                                                      matchResultForAtRegion.MatchedElements().at(0),
                                                      matchResultForAtRegion.MatchedElements().at(1),
                                                      CRawEntry(),
                                                      { missingRegionViolation }));
                                endingContent.emplace_back(atLmaRegion);
                                localIterator++; // We need to position the iterator at the last parsed element.
                            }
                            else
                            {
                                auto atLmaRegion = std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputAtLmaRegion(
                                                      matchResultForAtRegion.MatchedElements().at(0),
                                                      matchResultForAtRegion.MatchedElements().at(1),
                                                      *potentialRegionName));
                                endingContent.emplace_back(atLmaRegion);
                                localIterator += 2; // We need to position the iterator at the last parsed element.
                            }
                        }
                        else
                        {
                            // This is the start of a new expression or statement, etc. We need to rewind the iterator
                            // and return back to the caller.
                            localIterator--;
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

            case RawEntryType::ArithmeticOperator:
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
                            auto oneEntryAhead = FindNextNonCommentEntry(linkerScriptFile, iterator + 1, endOfVectorIterator);
                            if ((oneEntryAhead == endOfVectorIterator) || (oneEntryAhead->EntryType() != RawEntryType::Word)) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            } else {
                                auto toVmaRegion = std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputToVmaRegion(*localIterator,
                                                                                *oneEntryAhead,
                                                                                {*localIterator, *oneEntryAhead},
                                                                                {}));
                                endingContent.emplace_back(toVmaRegion);
                                localIterator = oneEntryAhead;
                            }

                            // This could be 'At VMA' definition
                            if (oneEntryAhead->EntryType() == RawEntryType::Word) {
                                SharedPtrVector<CViolationBase> toVmaRegionViolations;

                                if (CParserHelpers::IsReservedWord(linkerScriptFile.ResolveRawEntry(*oneEntryAhead))) {
                                    toVmaRegionViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, *oneEntryAhead}, EParserViolationCode::VmaRegionNameCannotBeAReservedWord)));
                                }

                                auto toVmaRegion = std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputToVmaRegion(*localIterator,
                                                                                *oneEntryAhead,
                                                                                {*localIterator, *oneEntryAhead},
                                                                                std::move(toVmaRegionViolations)));

                                endingContent.emplace_back(toVmaRegion);
                                localIterator = oneEntryAhead;
                            } else {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
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

            case RawEntryType::AssignmentOperator:
            {
                switch (parserState)
                {

                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingColon:
                    {
                        return nullptr; // This is not a valid Section Output command.
                    }

                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        auto oneEntryAhead = iterator + 1;
                        if ((oneEntryAhead == endOfVectorIterator) || (oneEntryAhead->EntryType() != RawEntryType::Number) || (resolvedContent != "=")) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));

                        } else {
                            SharedPtrVector<CViolationBase> programHeaderViolations;
                            auto resolvedOneEntryAhead = linkerScriptFile.ResolveRawEntry(*oneEntryAhead);
                            auto fillExpression = std::shared_ptr<CLinkerScriptContentBase>(
                                    new CSectionOutputFillExpression(*localIterator,
                                                           *oneEntryAhead,
                                                           {*localIterator, *oneEntryAhead},
                                                           std::move(programHeaderViolations)));
                            endingContent.emplace_back(fillExpression);
                            localIterator = oneEntryAhead;
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

            case RawEntryType::Colon:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr; // This is not a valid Section Output command.
                    }

                    case ParserState::AwaitingColon:
                    {
                        colonEntry = *iterator;
                        parserState = ParserState::AwaitingBracketOpen;
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        auto oneEntryAhead = iterator + 1;
                        if ((oneEntryAhead == endOfVectorIterator) || (oneEntryAhead->EntryType() != RawEntryType::Word)) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));

                        } else {
                            SharedPtrVector<CViolationBase> programHeaderViolations;
                            auto resolvedOneEntryAhead = linkerScriptFile.ResolveRawEntry(*oneEntryAhead);

                            if (CParserHelpers::IsReservedWord(resolvedOneEntryAhead)) {
                                programHeaderViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, *oneEntryAhead}, EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord)));
                            }

                            auto phdrCall = std::shared_ptr<CSectionOutputPhdr>(
                                    new CSectionOutputPhdr(*localIterator,
                                                           *oneEntryAhead,
                                                           {*localIterator, *oneEntryAhead},
                                                           std::move(programHeaderViolations)));

                            endingContent.emplace_back(phdrCall);
                            localIterator = oneEntryAhead;
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

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr; // This is not a valid Section Output command.
                    }

                    case ParserState::AwaitingColon:
                    {
                        std::vector<std::string> validTypes = {
                            "NOLOAD",
                            "DSECT",
                            "COPY",
                            "INFO",
                            "OVERLAY",
                        };

                        auto matchResult = MatchSequenceAnyContentWithinEnclosure(linkerScriptFile, iterator, endOfVectorIterator, "(", validTypes, ")");
                        if (matchResult.Successful()) {
                            auto matchedElements = matchResult.MatchedElements();
                            auto sectionOutputType = std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputType(matchedElements[1], matchedElements[0], matchedElements[2], std::move(matchedElements), {}));
                            preColonContent.emplace_back(sectionOutputType);
                            parserState = ParserState::AwaitingColon;

                        } else {
                            auto parsedContent =  std::dynamic_pointer_cast<CLinkerScriptContentBase>(expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                            if (parsedContent == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            } else {
                                preColonContent.emplace_back(parsedContent);
                            }
                        }

                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        localIterator = previousPositionIterator;
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

            case RawEntryType::QuestionMark:
            case RawEntryType::Comma:
            case RawEntryType::Number:
            case RawEntryType::String:            
            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr; // This is not a valid Section Output command.
                    }

                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        // This is the start of a new expression or statement, etc. We need to rewind the iterator
                        // and return back to the caller.
                        localIterator--;
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
    std::copy(parsingStartIteratorPosition, localIterator, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CSectionOutputCommand>(
                new CSectionOutputCommand(sectionOutputNameEntry,
                                          std::move(preColonContent),
                                          std::move(postColonContent),
                                          colonEntry,
                                          bracketOpenEntry,
                                          std::move(innerContent),
                                          bracketCloseEntry,
                                          std::move(endingContent),
                                          std::move(rawEntries),
                                          std::move(violations)));
}
