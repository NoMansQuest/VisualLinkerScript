#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CSectionOverlayParser.h"
#include "CExpressionParser.h"
#include "CSectionOutputCommandParser.h"
#include "CAssignmentProcedureParser.h"
#include "CInputSectionFunctionKeepParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "Constants.h"
#include "SubParserHelpers.h"

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
		const CLinkerScriptFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    LinqVector<CViolationBase> violations;

    CExpressionParser expressionParser;
    CFunctionParser functionParser;                                 // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                             // Example: '. = ALIGN(4);'
    CAssignmentProcedureParser assignmentProcedureParser;           // Example: PROVIDE(a = b);'
    CSectionOutputCommandParser inputSectionStatementParser;        // Example: A section-output command
    CInputSectionFunctionKeepParser inputSectionFunctionKeepParser; // Example: KEEP(*(SORT(*.txt.o)))

    auto parserState = ParserState::AwaitingHeader;

    CRawEntry overlayHeaderEntry;
    CRawEntry colonEntry;
    CRawEntry noCrossRefsEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    CRawEntry noCrossRefs;
    LinqVector<CParsedContentBase> preColonContent;
    LinqVector<CParsedContentBase> postColonContent;
    LinqVector<CParsedContentBase> innerContent;
    LinqVector<CParsedContentBase> endingContent;

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
                violations.emplace_back(std::make_shared<CParserViolation>(overlayHeaderEntry, EParserViolationCode::SectionOutputDefinitionIncomplete));
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
                    "ParserState invalid in CSectionOverlayParser");
            }
        }

        std::vector<CRawEntry>::const_iterator pushedLocalIterator = localIterator;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);

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

            case RawEntryType::Wildcard:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingColon:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        auto parsedStatement = inputSectionStatementParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                        if (parsedStatement == nullptr) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedStatement));
                        }
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
                                    "ParserState invalid in CSectionOverlayParser.");
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
                        std::vector<CRawEntry>::const_iterator copyOfIterator = localIterator;
                        auto fusedEntry = FuseEntriesToFormAWilcardWord(linkerScriptFile, copyOfIterator, endOfVectorIterator);
                        auto resolvedFusedEntry = linkerScriptFile.ResolveRawEntry(fusedEntry);

                        if (CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent)) {
                            // Such as 'CREATE_OBJECT_SYMBOLS'
                            innerContent.emplace_back(std::shared_ptr<CParsedContentBase>(new CSectionOutputDataExpression(*localIterator)));
                        } else if (CParserHelpers::IsAssignmentProcedure(resolvedContent)) {
                            // Such as "PROVIDE(....)"
                            auto parsedAssignmentProcedure = assignmentProcedureParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                            if (parsedAssignmentProcedure == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedAssignmentProcedure));
                            }
                        } else if (CParserHelpers::IsOutputSectionDataFunctionName(resolvedContent)) {
                            // Such as "BYTE(1)", "ALIGN(4)", etc.
                            auto parsedFunctionCall = functionParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                            if (parsedFunctionCall == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedFunctionCall));
                            }
                        } else if (CParserHelpers::IsKeepFunction(resolvedFusedEntry)) {
                            // KEEP function call.
                            auto parsedKeepFunctionCall = inputSectionFunctionKeepParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                            if (parsedKeepFunctionCall == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName)));
                            } else  {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedKeepFunctionCall));
                            }
                        } else if (CParserHelpers::IsSortFunction(resolvedFusedEntry)) {
                            // This is most likely an input section definition.
                            auto inputSectionStatement = inputSectionStatementParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (inputSectionStatement == nullptr) {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            } else {
                                innerContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(inputSectionStatement));
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
                        auto matchResultForAtRegion = MatchSequenceOpenEnded(linkerScriptFile, iterator, endOfVectorIterator, {"AT", ">"});
                        if (matchResultForAtRegion.Successful())
                        {
                            auto potentialRegionName = FindNextNonCommentEntry(linkerScriptFile, iterator + 2, endOfVectorIterator);
                            if (potentialRegionName == endOfVectorIterator) {
                                std::shared_ptr<CViolationBase> missingRegionViolation(new CParserViolation({*localIterator, matchResultForAtRegion.MatchedElements()[1]}, EParserViolationCode::MissingRegionForAtLmaDefinition));
                                auto atLmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputAtLmaRegion(
                                                      matchResultForAtRegion.MatchedElements()[0],
                                                      matchResultForAtRegion.MatchedElements()[1],
                                                      CRawEntry(),
                                                      { missingRegionViolation }));
                                endingContent.emplace_back(atLmaRegion);
                                localIterator++; // We need to position the iterator at the last parsed element.

                            } else {
                                auto atLmaRegion = std::shared_ptr<CParsedContentBase>(new CSectionOutputAtLmaRegion(
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

        	case RawEntryType::Colon:
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

                    case ParserState::AwaitingColon:
                    {
                        if (!StringEquals(resolvedContent, "-") && !StringEquals(resolvedContent, "+"))
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            break;
                        }

                        // If it's colon, then we're good to go anyway...
                        if (localIterator->EntryType() == RawEntryType::Colon)
                        {
                            colonEntry = *localIterator;
                            parserState = ParserState::AwaitingBracketOpen;
                            break;
                        }

                        // We are in an expression, most likely the address
                        // Here the job is a bit difficult. We need to extract
                        // 1 - [Optional] The 'startAddress' in form of an expression
                        // 2 - [Optional] The 'Block(align)' function call
                        // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                        auto parsedContent =  std::dynamic_pointer_cast<CParsedContentBase>(expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator));
                        if (parsedContent == nullptr) {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        } else {
                            preColonContent.emplace_back(parsedContent);
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
                        else if (localIterator->EntryType() == RawEntryType::Colon)
                        {
                            // This could be 'At VMA' definition
                            if ((oneEntryAhead.IsPresent()) && (oneEntryAhead.EntryType() == RawEntryType::Word))
                            {
                                auto resolvedEntryAhead = linkerScriptFile.ResolveRawEntry(oneEntryAhead);
                                LinqVector<CViolationBase> programHeaderViolations;

                                if (CParserHelpers::IsReservedWord(resolvedEntryAhead))
                                {
                                    programHeaderViolations.emplace_back(std::shared_ptr<CViolationBase>(
                                        new CParserViolation({*localIterator, oneEntryAhead}, EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord)));
                                }

                                std::shared_ptr<CSectionOutputPhdr> programHeader(
                                            new CSectionOutputPhdr(*localIterator,
                                                                   oneEntryAhead,
                                                                   {*localIterator, oneEntryAhead},
                                                                    programHeaderViolations));

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
                            --localIterator;
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
                        bracketOpenEntry = *localIterator;
                        parserState = ParserState::AwaitingBracketClosure;
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

    return std::shared_ptr<CSectionOverlayCommand>(
                new CSectionOverlayCommand(overlayHeaderEntry,
                                           colonEntry,
                                           noCrossRefsEntry,                                           
                                           bracketOpenEntry,
                                           bracketCloseEntry,
                                           preColonContent,
                                           postColonContent,
                                           innerContent,
                                           endingContent,
                                           rawEntries,
                                           violations));
}
