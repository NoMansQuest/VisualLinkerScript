#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CSectionOutputCommandParser.h"
#include "CExpressionParser.h"
#include "CInputSectionStatementParser.h"
#include "CInputSectionFunctionKeepParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "CAssignmentProcedureParser.h"
#include "Constants.h"
#include "SubParserHelpers.h"

#include "../CMasterParserException.h"

#include "../../Models/CInputSectionStatement.h"
#include "../../Models/CFunctionCall.h"
#include "../../Models/CSectionOutputConstraint.h"
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
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    LinqVector<CViolationBase> violations;

    CExpressionParser preColonExpressionParser(false, false, true);
    CFunctionParser functionParser;                             // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                         // Example: '. = ALIGN(4);'
    CAssignmentProcedureParser assignmentProcedureParser;       // Example: PROVIDE(a = b);'
    CInputSectionStatementParser inputSectionStatementParser;   // Example: 'foo.o (.input2)'
    CInputSectionFunctionKeepParser keepParser;                 // Example: KEEP(*(.isr_vector))

    auto parserState = ParserState::AwaitingHeader;

    CRawEntry sectionOutputNameEntry;
    CRawEntry colonEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    CRawEntry noCrossRefsEntry;
    LinqVector<CParsedContentBase> preColonContent;
    LinqVector<CParsedContentBase> postColonContent;
    LinqVector<CParsedContentBase> innerContent;
    LinqVector<CParsedContentBase> endingContent;

    std::vector<std::string> validTypes = {
    "NOLOAD",
    "DSECT",
    "COPY",
    "INFO",
    "OVERLAY",
    };


    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        // Edge case coverage, where we read end-of-file prematurely.
        if (localIterator == endOfVectorIterator)
        {
            switch (parserState)
            {
            case ParserState::AwaitingHeader:
            case ParserState::AwaitingColon:
                return nullptr;

            case ParserState::AwaitingBracketOpen:
                violations.emplace_back(std::make_shared<CParserViolation>(sectionOutputNameEntry, EParserViolationCode::SectionOutputDefinitionIncomplete));
                --localIterator;
                break;

            case ParserState::AwaitingBracketClosure:
                violations.emplace_back(std::make_shared<CParserViolation>(bracketOpenEntry, EParserViolationCode::SectionOutputBracketClosureMissing));
                --localIterator;
                break;

            case ParserState::AwaitingEndOfParse:
            {
                // This is the start of a new expression or statement, etc. We need to rewind the iterator
                // and return back to the caller.
                --localIterator;
                parserState = ParserState::ParsingComplete;
            }

            default:
                throw CMasterParsingException(
                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                    "ParserState invalid in CSectionOutputCommandParser");
            }
        }

        auto rawEntry = *localIterator;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(rawEntry);
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                std::shared_ptr<CParsedContentBase> commentObject(new CComment({*localIterator}, {}));
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

            case RawEntryType::Wildcard:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        // This is the start of a new expression or statement, etc. We need to rewind the iterator
                        // and return back to the caller.
                        --localIterator;
                        parserState = ParserState::ParsingComplete;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuCommandParser");
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
                        if (CParserHelpers::IsReservedWord(resolvedContent)) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword)));
                        }

                        parserState = ParserState::AwaitingColon;
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        if (StringEquals(resolvedContent, "NOCROSSREFS", true)) {
                            if (!noCrossRefsEntry.IsPresent()) {
                                noCrossRefsEntry = *localIterator;
                            } else {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::NoCrossRefsAlreadyDeclared)));
                            }
                        } else if (StringEquals(resolvedContent, "AT", true)) {
                            auto parsedContent =  std::dynamic_pointer_cast<CParsedContentBase>(functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                            if (parsedContent == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            } else {
                                postColonContent.emplace_back(parsedContent);
                            }
                        } else if (CParserHelpers::IsSectionOutputConstraint(resolvedContent)) {
                            postColonContent.emplace_back(std::shared_ptr<CParsedContentBase>(new CSectionOutputConstraint(*localIterator)));
                        }
                        break;
                    }

                    case ParserState::AwaitingColon:
                    {
                        auto matchResult = MatchSequenceAnyContent(linkerScriptFile, localIterator, endOfVectorIterator, validTypes);
                        if (matchResult.Successful())
                        {
                            auto matchedElements = matchResult.MatchedElements();
                            auto sectionOutputType = std::shared_ptr<CParsedContentBase>(new CSectionOutputType(*localIterator, { *localIterator }, {}));
                            preColonContent.emplace_back(sectionOutputType);
                            localIterator = matchResult.IteratorToLastElement();
                            parserState = ParserState::AwaitingColon;
                        }
                        else 
                        {
                            // Here the job is a bit difficult. We need to extract
                            // 1 - [Optional] The 'startAddress' in form of an expression
                            // 2 - [Optional] The 'Block(align)' function call
                            // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                            auto parsedContent = std::dynamic_pointer_cast<CParsedContentBase>(preColonExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                            if (parsedContent == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                            else {
                                preColonContent.emplace_back(parsedContent);
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        auto nextNonCommentEntryIterator = FindNextNonCommentEntry(linkerScriptFile, localIterator + 1, endOfVectorIterator);
                        auto entryPlusOne = (nextNonCommentEntryIterator != endOfVectorIterator) ? *nextNonCommentEntryIterator : CRawEntry();

                        if (CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent)) {
                            // Such as 'CREATE_OBJECT_SYMBOLS'
                            innerContent.emplace_back(std::shared_ptr<CParsedContentBase>(new CSectionOutputDataExpression(*localIterator)));

                        } else if (CParserHelpers::IsKeepFunction(resolvedContent)) {
                            // Such as 'KEEP(*(.isr_vector))'
                            if (entryPlusOne.EntryType() == RawEntryType::ParenthesisOpen) {
                                auto parsedKeepFunctionCall = keepParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedKeepFunctionCall == nullptr) {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                } else {
                                    innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedKeepFunctionCall));
                                }
                            } else {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            }

                        } else if (CParserHelpers::IsAssignmentProcedure(resolvedContent)) {
                            // Such as "PROVIDE(....)"
                            auto parsedAssignmentProcedure = assignmentProcedureParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedAssignmentProcedure == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedAssignmentProcedure));
                            }

                        } else if (CParserHelpers::IsOutputSectionDataFunctionName(resolvedContent)) {
                            // Such as "BYTE(1)", "ALIGN(4)", etc.
                            auto parsedFunctionCall = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedFunctionCall == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedFunctionCall));
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
                                    innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(inputSectionStatement));
                                }
                            } else {
                                innerContent.emplace_back( std::dynamic_pointer_cast<CParsedContentBase>(parsedAssignment));
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        auto matchResultForAtLmaRegion = MatchSequenceOpenEnded(linkerScriptFile, localIterator, endOfVectorIterator, {"AT", ">"});
                        auto matchResultForAtVmaRegion = MatchSequenceOpenEnded(linkerScriptFile, localIterator, endOfVectorIterator, { ">" });
                        if (matchResultForAtLmaRegion.Successful()) {

                            auto potentialRegionName = FindNextNonCommentEntry(linkerScriptFile, localIterator + 2, endOfVectorIterator);
                            if (potentialRegionName == endOfVectorIterator) {
                                std::shared_ptr<CViolationBase> missingRegionViolation(new CParserViolation({*localIterator, matchResultForAtLmaRegion.MatchedElements()[1]}, EParserViolationCode::MissingRegionForAtLmaDefinition));
                                auto atLmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputAtLmaRegion(
													  matchResultForAtLmaRegion.MatchedElements()[0],
										matchResultForAtLmaRegion.MatchedElements()[1],
                                                      CRawEntry(),
                                                      { missingRegionViolation }));
                                endingContent.emplace_back(atLmaRegion);
                                ++localIterator; // We need to position the iterator at the last parsed element.

                            } else {
                                auto atLmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputAtLmaRegion(
													  matchResultForAtLmaRegion.MatchedElements()[0],
										matchResultForAtLmaRegion.MatchedElements()[1],
                                                      *potentialRegionName));
                                endingContent.emplace_back(atLmaRegion);
                                localIterator += 2; // We need to position the iterator at the last parsed element.

                            }
                        }
                        else if (matchResultForAtVmaRegion.Successful())
                        {
                            auto potentialRegionName = FindNextNonCommentEntry(linkerScriptFile, localIterator + 1, endOfVectorIterator);
                            if (potentialRegionName == endOfVectorIterator) 
                            {
                                std::shared_ptr<CViolationBase> missingRegionViolation(new CParserViolation({ *localIterator, matchResultForAtLmaRegion.MatchedElements()[0] }, EParserViolationCode::MissingRegionForAtVmaDefinition));
                                auto atLmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputToVmaRegion(
                                    matchResultForAtLmaRegion.MatchedElements()[0],
                                    CRawEntry(),
                                    { matchResultForAtLmaRegion.MatchedElements()[0] },
                                    { missingRegionViolation }));
                                endingContent.emplace_back(atLmaRegion);
                            }
                            else 
                            {
                                auto atLmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputToVmaRegion(
                                    matchResultForAtLmaRegion.MatchedElements()[0],
                                    *potentialRegionName,
                                    { matchResultForAtLmaRegion.MatchedElements()[0], *potentialRegionName }, 
                                    {}));
                                endingContent.emplace_back(atLmaRegion);
                                localIterator += 1; // We need to position the iterator at the last parsed element.
                            }
                        }
                        else 
                        {
                            // This is the start of a new expression or statement, etc. We need to rewind the iterator
                            // and return back to the caller.
                            --localIterator;
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

                    case ParserState::AwaitingColon:
                    {
                        if (!StringEquals(resolvedContent, "-") && !StringEquals(resolvedContent, "+")) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            break;
                        }

                        // We are in an expression, most likely the address
                        // Here the job is a bit difficult. We need to extract
                        // 1 - [Optional] The 'startAddress' in form of an expression
                        // 2 - [Optional] The 'Block(align)' function call
                        // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                        auto parsedContent =  std::dynamic_pointer_cast<CParsedContentBase>(preColonExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                        if (parsedContent == nullptr) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            preColonContent.emplace_back(parsedContent);
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        break;
                    }
                    case ParserState::AwaitingBracketClosure:
                    {
                        // This could be a InputSectionStatement. Try parsing it...
                        auto inputSectionStatement = inputSectionStatementParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (inputSectionStatement == nullptr) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(inputSectionStatement));
                        }
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        if (resolvedContent == ">") {
                            auto oneEntryAhead = FindNextNonCommentEntry(linkerScriptFile, localIterator + 1, endOfVectorIterator);

                            if ((oneEntryAhead == endOfVectorIterator) || (oneEntryAhead->EntryType() != RawEntryType::Word)) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));

                            } else {
                                LinqVector<CViolationBase> toVmaRegionViolations;

                                if (CParserHelpers::IsReservedWord(linkerScriptFile.ResolveRawEntry(*oneEntryAhead))) {
                                    auto parserViolation = new CParserViolation({*localIterator, *oneEntryAhead}, EParserViolationCode::VmaRegionNameCannotBeAReservedWord);
                                    toVmaRegionViolations.emplace_back(std::shared_ptr<CViolationBase>(parserViolation));
                                }

                                auto toVmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputToVmaRegion(*localIterator,
                                                                                *oneEntryAhead,
                                                                                {*localIterator, *oneEntryAhead},
                                                                                toVmaRegionViolations));
                                endingContent.emplace_back(toVmaRegion);
                                localIterator = oneEntryAhead;
                            }
                        }                      
                        else
                        {
                            --localIterator;
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
                        auto oneEntryAhead = localIterator + 1;
                        if ((oneEntryAhead == endOfVectorIterator) || (oneEntryAhead->EntryType() != RawEntryType::Number) || (resolvedContent != "=")) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            LinqVector<CViolationBase> programHeaderViolations;
                            auto resolvedOneEntryAhead = linkerScriptFile.ResolveRawEntry(*oneEntryAhead);
                            auto fillExpression = std::shared_ptr<CParsedContentBase>(
                                    new CSectionOutputFillExpression(*localIterator,
                                                           *oneEntryAhead,
                                                           {*localIterator, *oneEntryAhead},
                                                           programHeaderViolations));
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
                        colonEntry = *localIterator;
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
                        auto oneEntryAhead = localIterator + 1;
                        if ((oneEntryAhead == endOfVectorIterator) || (oneEntryAhead->EntryType() != RawEntryType::Word)) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));

                        } else {
                            LinqVector<CViolationBase> programHeaderViolations;
                            auto resolvedOneEntryAhead = linkerScriptFile.ResolveRawEntry(*oneEntryAhead);

                            if (CParserHelpers::IsReservedWord(resolvedOneEntryAhead)) {
                                programHeaderViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({*localIterator, *oneEntryAhead}, EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord)));
                            }

                            auto phdrCall = std::shared_ptr<CSectionOutputPhdr>(
                                    new CSectionOutputPhdr(*localIterator,
                                                           *oneEntryAhead,
                                                           {*localIterator, *oneEntryAhead},
                                                           programHeaderViolations));

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
                        auto matchResult = MatchSequenceAnyContentWithinEnclosure(linkerScriptFile, localIterator, endOfVectorIterator, "(", validTypes, ")");
                        if (matchResult.Successful())
                        {
                            auto matchedElements = matchResult.MatchedElements();
                            auto sectionOutputType = std::shared_ptr<CParsedContentBase>(new CSectionOutputType(
                                matchedElements[1], 
                                matchedElements[0], 
                                matchedElements[2],
                                matchedElements, 
                                {}));

                            preColonContent.emplace_back(sectionOutputType);
                            localIterator = matchResult.IteratorToLastElement();
                            parserState = ParserState::AwaitingColon;
                        }
                        else 
                        {
                            auto parsedContent = std::dynamic_pointer_cast<CParsedContentBase>(preColonExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                            if (parsedContent == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                            else {
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
                        --localIterator;
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

            case RawEntryType::Number:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    {
                        return nullptr; // This is not a valid Section Output command.
                    }

                    case ParserState::AwaitingColon:
                    {
                        auto parsedContent =  std::dynamic_pointer_cast<CParsedContentBase>(preColonExpressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                        if (parsedContent == nullptr) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            preColonContent.emplace_back(parsedContent);
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

            case RawEntryType::QuestionMark:
            case RawEntryType::Comma:            
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
                        --localIterator;
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
                        return nullptr;
                    }

                    case ParserState::AwaitingColon:
                    {
                        // Grounds to abort.                        
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
                        --localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionsCommandParser");
                }
                break;
            }

            case RawEntryType::BracketClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingColon:
                    {
                        // Grounds to abort.
                        return nullptr;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        bracketCloseEntry = *localIterator;
                        parserState = ParserState::AwaitingEndOfParse;
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        --localIterator;
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
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

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
    std::copy(parsingStartIteratorPosition, localIterator + 1, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CSectionOutputCommand>(
                new CSectionOutputCommand(sectionOutputNameEntry,
                                          preColonContent,
                                          postColonContent,
                                          colonEntry,
                                          noCrossRefsEntry,
                                          bracketOpenEntry,
                                          innerContent,
                                          bracketCloseEntry,
                                          endingContent,
                                          rawEntries,
                                          violations));
}
