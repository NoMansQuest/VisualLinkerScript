#include <vector>
#include <memory>
#include <string>
#include <iterator>

#include "CSectionsRegionParser.h"
#include "CSectionOutputCommandParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "CAssignmentProcedureParser.h"
#include "Constants.h"
#include "CSectionOverlayParser.h"

#include "../CMasterParserException.h"

#include "../../Models/CFunctionCall.h"
#include "../../Models/CSectionsRegion.h"
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
    /// @brief Parse states for Sections Region Parser
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingBracketOpen,
        AwaitingBracketClosure,
        ParsingComplete
    };
}

std::shared_ptr<CSectionsRegion> CSectionsRegionParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    SharedPtrVector<CViolationBase> violations;

    CFunctionParser functionParser;                             // Example: FILL(0x00000)
    CAssignmentParser assignmentParser;                         // Example: '. = ALIGN(4);'
    CAssignmentProcedureParser assignmentCommandParser;         // Example: PROVIDE(a = b + c);
    CSectionOutputCommandParser sectionOutputCommandParser;     // Example: 'foo.o (.input2)'
    CSectionOverlayParser sectionOverlayParser;                 // Example: OVERLAY...

    auto parserState = ParserState::AwaitingHeader;

    CRawEntry headerEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);

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
                        if (StringEquals(resolvedContent, "SECTIONS", true))
                        {
                            headerEntry = *localIterator;
                            parserState = ParserState::AwaitingBracketOpen;
                        }
                        else
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword)));
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
                        if (resolvedContent == "OVERLAY")
                        {
                            // Such as 'CREATE_OBJECT_SYMBOLS'
                            auto parsedOverlayStatement = sectionOverlayParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedOverlayStatement == nullptr)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::UnableToUnderstandOverlaySection)));
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedOverlayStatement));
                            }
                        }
                        else if (CParserHelpers::IsOutputSectionDataFunctionName(resolvedContent))
                        {                            
                            auto parsedFunction = functionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedFunction == nullptr)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedFunction));
                            }
                        }
                        else if (CParserHelpers::IsAssignmentProcedure(resolvedContent))
                        {                            
                            auto parsedAssignmentProcedure = assignmentCommandParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedAssignmentProcedure == nullptr)
                            {
                                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedAssignmentProcedure));
                            }
                        }
                        else
                        {
                            // Is this an "Assignment"?
                            auto parsedAssignment = assignmentParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                            if (parsedAssignment == nullptr)
                            {
                                // Is Section Output Command?
                                auto sectionOutputCommand = sectionOutputCommandParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (sectionOutputCommand == nullptr)
                                {
                                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                                }
                                else
                                {
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(sectionOutputCommand));
                                }
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedAssignment));
                            }
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionsRegionParser");
                }

                break;
            }

            case RawEntryType::ArithmeticOperator:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
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
                        return nullptr;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        bracketOpenEntry = *localIterator;
                        parserState = ParserState::AwaitingBracketClosure;
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        localIterator--;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionsRegionParser");
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

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionsRegionParser");
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

    return std::shared_ptr<CSectionsRegion>(
                new CSectionsRegion(headerEntry,
                                    bracketOpenEntry,
                                    bracketCloseEntry,
                                    std::move(parsedContent),
                                    std::move(rawEntries),
                                    std::move(violations)));
}
