
#include <vector>
#include <memory>
#include <iterator>

#include "CMemoryStatementAttributeParser.h"
#include "../CMasterParserException.h"
#include "../../Models/Raw/CRawEntry.h"
#include "../../Models/CComment.h"
#include "../../Models/CMemoryStatementAttribute.h"
#include "../CParserViolation.h"
#include "../EParserViolationCode.h"
#include "../../Helpers.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

namespace
{
    /// @brief CPhdrsRegionContentParser parser states
    enum class ParserState
    {
        AwaitingParenthesisOverture,
        AwaitingParenthesisClosure,
        ParsingComplete
    };

    /// @brief Process a single character in attributes word (this is mostly to save repetitive coding).
    void ProcessAttribute(CRawEntry rawEntry,
                          bool negatingSymbolDetected,
                          AttributeDefinitionState& sectionToAssign,
                          SharedPtrVector<CViolationBase>& violations,
                          EParserViolationCode violationCode)
    {
        if (sectionToAssign != AttributeDefinitionState::Undefined)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({ rawEntry }, violationCode)));
            return;
        }

         sectionToAssign = negatingSymbolDetected ? AttributeDefinitionState::DefinedAndNegated : AttributeDefinitionState::Defined;
    }

    /// @brief Parses and extracts attributes from the given CRawEntry.
    void ParseAttributeValues(const CRawEntry& rawEntry,
                              const std::string& resolvedContet,
                              AttributeDefinitionState& readOnlySection,
                              AttributeDefinitionState& readWriteSection,
                              AttributeDefinitionState& allocatableSection,
                              AttributeDefinitionState& executableSection,
                              AttributeDefinitionState& initializedSection,
                              SharedPtrVector<CViolationBase>&  detectedViolations) noexcept
    {
        auto inversion = false;

        for (auto character: resolvedContet)
        {
            switch (character)
            {
                case 'R':
                case 'r':
                {
                    ProcessAttribute(rawEntry, inversion, readOnlySection, detectedViolations, EParserViolationCode::ReadAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'W':
                case 'w':
                {
                    ProcessAttribute(rawEntry, inversion, readWriteSection, detectedViolations, EParserViolationCode::ReadWriteAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'A':
                case 'a':
                {
                    ProcessAttribute(rawEntry, inversion, allocatableSection, detectedViolations, EParserViolationCode::AlloctableAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'X':
                case 'x':
                {
                    ProcessAttribute(rawEntry, inversion, executableSection, detectedViolations, EParserViolationCode::ExecutableAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'I':
                case 'i':
                case 'L':
                case 'l':
                {
                    ProcessAttribute(rawEntry, inversion, initializedSection, detectedViolations, EParserViolationCode::InitializedAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case '!':
                {
                    if (inversion)
                    {
                        detectedViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({ rawEntry }, EParserViolationCode::NegatingSymbolIsAlreadySet)));
                        break;
                    }

                    inversion = true;
                    break;
                }

                default:
                {
                    detectedViolations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation({ rawEntry }, EParserViolationCode::UnrecognizedAttributeSymbolWasFound)));
                    break;
                }
            }
        }
    }
}


std::shared_ptr<CMemoryStatementAttribute> CMemoryStatementAttributeParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator)
{
    auto localIterator = iterator;
    auto parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    if (iterator->EntryType() != RawEntryType::ParenthesisOpen)
    {
        throw CMasterParsingException(
                MasterParsingExceptionType::InternalParserError,
                "'CMemoryStatementAttributeParser::TryParse' can only be called with 'iterator' pointing to a 'Parenthesis-Open'");
    }

    auto parserState = ParserState::AwaitingParenthesisOverture;
    auto negatorArmed = false;

    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;
    AttributeDefinitionState readOnlySection;     // Value: R
    AttributeDefinitionState readWriteSection;    // Value: W
    AttributeDefinitionState allocatableSection;  // Value: A
    AttributeDefinitionState executableSection;   // Value: X
    AttributeDefinitionState initializedSection;  // Value: I or L

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        if (lineChangeDetected)
        {
            // Any line-change would be rended parsing attempt null and void (however, it may be possible to report
            // back a type of statement)
            localIterator--;
            parserState = ParserState::ParsingComplete;
            break;
        }

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Wildcard:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr;
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::WildcardsNotAllowedHere)));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryStatementAttributeParser.");
                }
                break;
            }

            case RawEntryType::Word:
            {                
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        return nullptr; // Aborting
                    }

                    case ParserState::AwaitingParenthesisClosure:
                    {
                        ParseAttributeValues(
                                    *localIterator,
                                     resolvedContent,
                                     readOnlySection,
                                     readWriteSection,
                                     allocatableSection,
                                     executableSection,
                                     initializedSection,
                                     violations);
                        break;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CMemoryStatementAttributeParser");
                    }
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisOverture:
                    {
                        parenthesisOpen = *localIterator;
                        parserState = ParserState::AwaitingParenthesisClosure;
                        break;
                    }
                    default:
                    {
                        return nullptr; // Parsing failed
                    }
                }
                break;
            }

            case RawEntryType::ParenthesisClose:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisClosure:
                    {
                        parenthesisClose = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }
                    default:
                    {
                        return nullptr; // Parsing failed
                    }
                }
                break;
            }

            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                // This needs to be handled by the CMemoryRegionParser, and not here...
                localIterator--;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::ArithmeticOperator:
            {
                if (resolvedContent != "!")
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                    break;
                }

                if (negatorArmed)
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::NegatingSymbolIsAlreadySet)));
                }

                negatorArmed = true;
                break;
            }

            case RawEntryType::Colon:
            case RawEntryType::QuestionMark:
            case RawEntryType::Semicolon:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::Comma:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::Number:
            case RawEntryType::String:            
            case RawEntryType::Unknown:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::NotPresent:
            {
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'non-present' entry was detected.");
            }

            default:
            {
                throw CMasterParsingException(
                        MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound,
                        "Unrecognized raw-entry type detected.");
            }
        }

        localIterator += ((parserState != ParserState::ParsingComplete) ? 1 : 0);
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator + 1, std::back_inserter(rawEntries));

    iterator = localIterator;

    return std::shared_ptr<CMemoryStatementAttribute>(
                new CMemoryStatementAttribute(parenthesisOpen,
                                              parenthesisClose,
                                              readOnlySection,
                                              readWriteSection,
                                              allocatableSection,
                                              executableSection,
                                              initializedSection,
                                              std::move(rawEntries),
                                              std::move(violations)));
}
