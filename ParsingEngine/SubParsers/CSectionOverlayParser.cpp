#include <vector>
#include <memory>
#include <string>
#include "CSectionOverlayParser.h"
#include "CExpressionParser.h"
#include "CSectionOutputCommandParser.h"
#include "CAssignmentProcedureParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "Constants.h"
#include "../CMasterParserException.h"
#include "../Models/CFunctionCall.h"
#include "../Models/CSectionOutputPhdr.h"
#include "../Models/CSectionOutputFillExpression.h"
#include "../Models/CSectionOverlayCommand.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

/*
 * Structure:
 *
 * OVERLAY [start] : [NOCROSSREFS] [AT ( ldaddr )]
 * {
 *   secname1
 *     {
 *       output-section-command
 *       output-section-command
 *       …
 *     } [:phdr…] [=fill]
 *   secname2
 *     {
 *       output-section-command
 *       output-section-command
 *       …
 *     } [:phdr…] [=fill]
 *   …
 * } [>region] [:phdr…] [=fill]
 *
 *
 *
 * Example:
 *
 * OVERLAY 0x1000 : AT (0x4000)
 * {
 *   .text0 { o1/*.o(.text) }
 *   .text1 { o2/*.o(.text) }
 * }
 *
 */

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
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<CViolation> violations;

    CExpressionParser expressionParser(ExpressionParserType::NormalParser, false);
    CFunctionParser functionParser;                             // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                         // Example: '. = ALIGN(4);'
    CAssignmentProcedureParser assignmentProcedureParser;       // Example: PROVIDE(a = b);'
    CSectionOutputCommandParser inputSectionStatementParser;    // Example: A section-output command

    auto parserState = ParserState::AwaitingHeader;
    auto doNotAdvance = false;

    CRawEntry sectionOutputNameEntry;
    std::shared_ptr<CExpression> startAddressExpression;
    std::shared_ptr<CFunctionCall> atAddressFunction;
    CRawEntry colonEntry;
    CRawEntry noCrossRefsEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<std::shared_ptr<CSectionOutputPhdr>> programHeaders;
    std::shared_ptr<CSectionOutputFillExpression> fillExpression;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;

        std::vector<CRawEntry>::const_iterator pushedLocalIterator = localIterator;
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
                parsedContent.emplace_back(commentObject);
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

                        sectionOutputNameEntry = *localIterator;
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::SectionOutputNameCannotBeAReservedKeyword));
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
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            if (parsedStartAddressExpression != nullptr)
                            {
                                startAddressExpression = parsedStartAddressExpression;
                            }
                            else
                            {
                                violations.emplace_back(CViolation(std::vector<CRawEntry>(previousPositionIterator, localIterator),
                                                                   ViolationCode::StartAddressExpressionAlreadyDeclared));
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:                    
                    {
                        // Here the job is a bit difficult. We need to extract
                        // 1 - [Optional] The 'startAddress' in form of an expression
                        // 2 - [Optional] The 'Block(align)' function call
                        // 3 - [Optional] The section-output type (e.g. (NOLOAD))
                        if (localIteratorPlusOne->EntryType() == RawEntryType::ParenthesisOpen)
                        {
                            auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (atAddressFunction == nullptr)
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                            }
                            else
                            {
                                if (parsedFunction == nullptr)
                                {
                                    atAddressFunction = parsedFunction;
                                }
                                else
                                {
                                    violations.emplace_back(CViolation(std::vector<CRawEntry>(pushedLocalIterator, localIterator),
                                                                       ViolationCode::AtAddressAlreadyDeclared));
                                    localIterator = pushedLocalIterator;
                                }
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
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::NoCrossRefsAlreadyDeclared));
                            }
                        }
                        else
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }

                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        if (CParserHelpers::IsInputSectionSpecialFunctionName(resolvedContent))
                        {
                            // Such as 'CREATE_OBJECT_SYMBOLS'
                            parsedContent.emplace_back();
                        }
                        else if (CParserHelpers::IsOutputSectionDataFunctionName(resolvedContent))
                        {
                            // Such as "BYTE(1)", "ALIGN(4)", etc.
                            parsedContent.emplace_back();
                        }
                        else if (CParserHelpers::IsAssignmentProcedure(resolvedContent))
                        {
                            // Such as: PROVIDE(a = b);
                            parsedContent.emplace_back();
                        }
                        else
                        {
                            // Is this an "Assignment"?
                            auto parsedAssignment = assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedAssignment == nullptr)
                            {
                                // Is this an InputSection definition?
                                auto inputSectionStatement = inputSectionStatementParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (inputSectionStatement == nullptr)
                                {
                                    violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                                }
                                else
                                {
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(inputSectionStatement));
                                }
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedAssignment));
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        if (resolvedContent == "AT")
                        {
                           if (resolvedContentPlusOne == ">")
                           {
                               if (!resolvedContentPlusTwo.empty())
                               {
                                   violations.emplace_back(CViolation({*localIterator, *localIteratorPlusOne, *localIteratorPlusTwo},
                                                                      ViolationCode::NotAllowedInOverlayCommand));
                                   localIterator = localIteratorPlusTwo;
                               }
                               else
                               {
                                   violations.emplace_back(CViolation({*localIterator, *localIteratorPlusOne},
                                                                      ViolationCode::NotAllowedInOverlayCommand));
                                   localIterator = localIteratorPlusOne;
                               }
                           }
                           else
                           {
                               violations.emplace_back(CViolation(*localIterator,
                                                                  ViolationCode::NotAllowedInOverlayCommand));
                           }
                        }
                        else
                        {
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
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        if (resolvedContent == ">")
                        {
                            if (!resolvedContentPlusOne.empty())
                            {
                                violations.emplace_back(CViolation({*localIterator, *localIteratorPlusOne}, ViolationCode::NotAllowedInOverlayCommand));
                                localIterator = localIteratorPlusOne;
                            }
                            else
                            {
                                violations.emplace_back(CViolation({*localIterator}, ViolationCode::NotAllowedInOverlayCommand));
                            }
                        }
                        else if (resolvedContent == ":")
                        {
                            // This could be 'At VMA' definition
                            if (rawEntryPlusOne.EntryType() == RawEntryType::Word)
                            {
                                std::vector<CViolation> programHeaderViolations;

                                if (CParserHelpers::IsReservedWord(resolvedContentPlusOne))
                                {
                                    programHeaderViolations.emplace_back(
                                                CViolation({*localIterator, rawEntryPlusOne}, ViolationCode::ProgramHeaderNameCannotBeAReservedWord));
                                }


                                std::shared_ptr<CSectionOutputPhdr> programHeader(
                                            new CSectionOutputPhdr(*localIterator,
                                                                   rawEntryPlusOne,
                                                                   {*localIterator, rawEntryPlusOne},
                                                                  std::move(programHeaderViolations)));

                                programHeaders.emplace_back(programHeader);
                                localIterator = localIteratorPlusOne;
                            }
                            else
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingProgramHeaderName));
                            }
                        }
                        else if (resolvedContent == "=")
                        {
                            // This could be 'At VMA' definition
                            if (rawEntryPlusOne.EntryType() == RawEntryType::Number)
                            {
                                fillExpression =  std::shared_ptr<CSectionOutputFillExpression>(
                                            new CSectionOutputFillExpression(*localIterator,
                                                                             rawEntryPlusOne,
                                                                             {*localIterator, rawEntryPlusOne},
                                                                             {}));
                                localIterator = localIteratorPlusOne;
                            }
                            else
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingANumericValueForFillExpression));
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
            }

            case RawEntryType::Assignment:
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
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            if (parsedStartAddressExpression != nullptr)
                            {
                                startAddressExpression = parsedStartAddressExpression;
                            }
                            else
                            {
                                violations.emplace_back(CViolation(std::vector<CRawEntry>(previousPositionIterator, localIterator),
                                                                   ViolationCode::StartAddressExpressionAlreadyDeclared));
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    case ParserState::AwaitingEndOfParse:
                    {
                        // This marks the end of our parsing
                        parserState = ParserState::ParsingComplete;
                        localIterator = previousPositionIterator;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
            }

            case RawEntryType::String:
            {
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
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
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else
                        {
                            if (parsedStartAddressExpression != nullptr)
                            {
                                startAddressExpression = parsedStartAddressExpression;
                            }
                            else
                            {
                                violations.emplace_back(CViolation(std::vector<CRawEntry>(previousPositionIterator, localIterator),
                                                                   ViolationCode::StartAddressExpressionAlreadyDeclared));
                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingEndOfParse:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionOutpuStatementParser");
                }
            }

            case RawEntryType::ParenthesisClose:
            {
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
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

    return std::shared_ptr<CSectionOverlayCommand>(
                new CSectionOverlayCommand(sectionOutputNameEntry,
                                           startAddressExpression,
                                           colonEntry,
                                           noCrossRefsEntry,
                                           atAddressFunction,
                                           bracketOpenEntry,
                                           bracketCloseEntry,
                                           std::move(programHeaders),
                                           fillExpression,
                                           std::move(parsedContent),
                                           std::move(rawEntries),
                                           std::move(violations)));
}
