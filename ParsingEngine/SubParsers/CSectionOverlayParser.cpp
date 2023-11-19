#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CSectionOverlayParser.h"
#include "CExpressionParser.h"
#include "CSectionOutputCommandParser.h"
#include "CAssignmentProcedureParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "Constants.h"

#include "../CMasterParserException.h"

#include "../../Models/CFunctionCall.h"
#include "../../Models/CSectionOutputPhdr.h"
#include "../../Models/CSectionOutputFillExpression.h"
#include "../../Models/CSectionOutputAtLmaRegion.h"
#include "../../Models/CSectionOverlayCommand.h"
#include "../../Models/CSectionOutputDataExpression.h"
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

std::shared_ptr<CSectionOverlayCommand> CSectionOverlayParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    SharedPtrVector<CViolationBase> violations;

    CExpressionParser expressionParser;
    CFunctionParser functionParser;                             // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                         // Example: '. = ALIGN(4);'
    CAssignmentProcedureParser assignmentProcedureParser;       // Example: PROVIDE(a = b);'
    CSectionOutputCommandParser inputSectionStatementParser;    // Example: A section-output command

    auto parserState = ParserState::AwaitingHeader;

    CRawEntry overlayHeaderEntry;
    CRawEntry colonEntry;
    CRawEntry noCrossRefsEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    CRawEntry noCrossRefs;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> preColonContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> postColonContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> innerContent;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> endingContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        std::vector<CRawEntry>::const_iterator pushedLocalIterator = localIterator;
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
                    {
                        innerContent.emplace_back(commentObject);
                        break;
                    }
                    case ParserState::AwaitingEndOfParse:
                    {
                        endingContent.emplace_back(commentObject);
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOverlayParser");
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
                        if (resolvedContent != "OVERLAY")
                        {
                            return nullptr; // Failed parsing
                        }

                        overlayHeaderEntry = *localIterator;
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword)));
                        }

                        parserState = ParserState::AwaitingColon;
                        break;
                    }

                    case ParserState::AwaitingColon:
                    {
                        // Here we have to parse "StartAddress"
                        auto parsedStartAddressExpression = expressionParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                        if (parsedStartAddressExpression == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                           preColonContent.emplace_back(parsedStartAddressExpression);
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:                    
                    {
                        // Here the job is a bit difficult. We need to extract
                        // 1 - [Optional] The 'startAddress' in form of an expression
                        // 2 - [Optional] The 'Block(align)' function call
                        // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                        if (StringEquals(resolvedContent, "AT"))
                        {
                            auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedFunction == nullptr)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                            else
                            {
                                postColonContent.emplace_back(parsedFunction);
                            }
                        }
                        else if (resolvedContent == "NOCROSSREFS")
                        {
                            if (!noCrossRefsEntry.IsPresent())
                            {
                                noCrossRefsEntry = *localIterator;
                            }
                            else
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::NoCrossRefsAlreadyDeclared)));
                            }
                        }
                        else
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        if (CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent)) {
                            // Such as 'CREATE_OBJECT_SYMBOLS'
                            innerContent.emplace_back(std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputDataExpression(*localIterator)));

                        } else if (CParserHelpers::IsAssignmentProcedure(resolvedContent)) {
                            // Such as "PROVIDE(....)"
                            auto parsedAssignmentProcedure = assignmentProcedureParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                            if (parsedAssignmentProcedure == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedAssignmentProcedure));
                            }

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
                                                      matchResultForAtRegion.MatchedElements()[0],
                                                      matchResultForAtRegion.MatchedElements()[1],
                                                      CRawEntry(),
                                                      { missingRegionViolation }));
                                endingContent.emplace_back(atLmaRegion);
                                localIterator++; // We need to position the iterator at the last parsed element.
                            }
                            else
                            {
                                auto atLmaRegion = std::shared_ptr<CLinkerScriptContentBase>(new CSectionOutputAtLmaRegion(
                                                      matchResultForAtRegion.MatchedElements()[0],
                                                      matchResultForAtRegion.MatchedElements()[1],
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

            case RawEntryType::Semicolon:
            case RawEntryType::Comma:
            case RawEntryType::QuestionMark:
            case RawEntryType::EvaluativeOperators:
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
                        auto oneEntryAheadIterator = FindNextNonCommentEntry(linkerScriptFile, iterator + 1, endOfVectorIterator);
                        auto oneEntryAhead = (oneEntryAheadIterator != endOfVectorIterator) ? *oneEntryAheadIterator : CRawEntry();

                        if ((resolvedContent == ">"))
                        {                            
                            if (oneEntryAhead.IsPresent())
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, oneEntryAhead}, EParserViolationCode::NotAllowedInOverlayCommand)));
                                localIterator = oneEntryAheadIterator;
                            }
                            else
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator}, EParserViolationCode::NotAllowedInOverlayCommand)));
                            }
                        }
                        else if (CParserHelpers::IsColon(resolvedContent))
                        {
                            // This could be 'At VMA' definition
                            if ((oneEntryAhead.IsPresent()) && (oneEntryAhead.EntryType() == RawEntryType::Word))
                            {
                                auto resolvedEntryAhead = linkerScriptFile.ResolveRawEntry(oneEntryAhead);
                                SharedPtrVector<CViolationBase> programHeaderViolations;

                                if (CParserHelpers::IsReservedWord(resolvedEntryAhead))
                                {
                                    programHeaderViolations.emplace_back(std::shared_ptr<CViolationBase>(
                                        new CParserViolation({*localIterator, oneEntryAhead}, EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord)));
                                }

                                std::shared_ptr<CSectionOutputPhdr> programHeader(
                                            new CSectionOutputPhdr(*localIterator,
                                                                   oneEntryAhead,
                                                                   {*localIterator, oneEntryAhead},
                                                                  std::move(programHeaderViolations)));

                                endingContent.emplace_back(programHeader);
                                localIterator = oneEntryAheadIterator;
                            }
                            else
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingProgramHeaderName)));
                            }
                        }
                        else
                        {
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

            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr; // Ground to abort
                    }

                    case ParserState::AwaitingColon:
                    {
                        // Here we have to parse "StartAddress"
                        auto parsedStartAddressExpression = expressionParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                        if (parsedStartAddressExpression == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {                            
                            preColonContent.emplace_back(parsedStartAddressExpression);
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        auto oneEntryAheadIterator = FindNextNonCommentEntry(linkerScriptFile, iterator + 1, endOfVectorIterator);
                        auto oneEntryAhead = (oneEntryAheadIterator != endOfVectorIterator) ? *oneEntryAheadIterator : CRawEntry();


                        if ((resolvedContent == "=") && (oneEntryAhead.IsPresent()))
                        {
                            // This could be 'At VMA' definition
                            if (oneEntryAhead.EntryType() == RawEntryType::Number)
                            {
                                auto fillExpression =  std::shared_ptr<CSectionOutputFillExpression>(
                                            new CSectionOutputFillExpression(*localIterator,
                                                                             oneEntryAhead,
                                                                             {*localIterator, oneEntryAhead},
                                                                             {}));
                                endingContent.emplace_back(fillExpression);
                                localIterator = oneEntryAheadIterator;
                            }
                            else
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WasExpectingANumericValueForFillExpression)));
                            }
                        }
                        else
                        {
                            // This marks the end of our parsing
                            localIterator--;
                            parserState = ParserState::ParsingComplete;                            
                            break;
                        }
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
                break;
            }

            case RawEntryType::String:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        // Grounds to abort.
                        return nullptr;
                    }

                    case ParserState::AwaitingColon:
                    {
                        // Here we have to parse "StartAddress"
                        auto parsedStartAddressExpression = expressionParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                        if (parsedStartAddressExpression == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            preColonContent.emplace_back(parsedStartAddressExpression);
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingEndOfParse:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::BracketOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        // Grounds to abort.
                        localIterator--;
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
                        localIterator--;
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

        localIterator += ((parserState != ParserState::ParsingComplete) ? 1 : 0);
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CSectionOverlayCommand>(
                new CSectionOverlayCommand(overlayHeaderEntry,
                                           colonEntry,
                                           noCrossRefsEntry,                                           
                                           bracketOpenEntry,
                                           bracketCloseEntry,
                                           std::move(preColonContent),
                                           std::move(postColonContent),
                                           std::move(innerContent),
                                           std::move(endingContent),
                                           std::move(rawEntries),
                                           std::move(violations)));
}
