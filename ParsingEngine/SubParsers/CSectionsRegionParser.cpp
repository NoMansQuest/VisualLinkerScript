#include <vector>
#include <memory>
#include <string>
#include <iterator>
#include <QDebug>

#include "CSectionsRegionParser.h"

#include <qlogging.h>

#include "CSectionOutputCommandParser.h"
#include "CFunctionParser.h"
#include "CAssignmentParser.h"
#include "CAssignmentProcedureParser.h"
#include "Constants.h"
#include "CSectionOverlayParser.h"

#include "../CMasterParserException.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"

#include "../../Models/CFunctionCall.h"
#include "../../Models/CSectionsRegion.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/Raw/RawEntryType.h"
#include "../../Models/Raw/CRawFile.h"
#include "../../Models/CComment.h"

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
		const CLinkerScriptFile& linkerScriptFile,
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
    std::vector<std::shared_ptr<CParsedContentBase>> parsedContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        // Edge case coverage, where we read end-of-file prematurely.
        if (localIterator == endOfVectorIterator)
        {
            switch (parserState)
            {
            case ParserState::AwaitingHeader:
            case ParserState::AwaitingBracketOpen:
                return nullptr;
                            
            case ParserState::AwaitingBracketClosure:
                violations.emplace_back(std::make_shared<CParserViolation>(bracketOpenEntry, EParserViolationCode::SectionsRegionBracketClosureMissing));
                --localIterator;
                break;

            default:
                throw CMasterParsingException(
                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                    "ParserState invalid in CSectionsRegionParser");
            }
        }

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                std::shared_ptr<CParsedContentBase> commentObject(new CComment({*localIterator}, {}));
                parsedContent.emplace_back(commentObject);
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

                    case ParserState::AwaitingBracketOpen:
                    case ParserState::AwaitingBracketClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CSectionRegionParser.");
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
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedOverlayStatement));
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
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedFunction));
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
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedAssignmentProcedure));
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
                                    parsedContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(sectionOutputCommand));
                                }
                            }
                            else
                            {
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CParsedContentBase>(parsedAssignment));
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

            
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::Comma:
            case RawEntryType::QuestionMark:
            case RawEntryType::ArithmeticOperator:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::Colon:            
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::Semicolon:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryCanBeOmitted, ESeverityCode::Information)));
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
                        --localIterator;
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

    return std::shared_ptr<CSectionsRegion>(
                new CSectionsRegion(headerEntry,
                                    bracketOpenEntry,
                                    bracketCloseEntry,
                                    parsedContent,
                                    rawEntries,
                                    violations));
}
