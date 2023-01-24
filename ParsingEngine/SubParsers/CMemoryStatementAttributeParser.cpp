
#include <vector>
#include <memory>
#include "CMemoryStatementAttributeParser.h"
#include "../CMasterParserException.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CComment.h"
#include "../Models/CMemoryStatementAttribute.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace
{
    /// @brief CPhdrsRegionContentParser parser states
    enum class ParserState
    {
        AwaitingParenthesisOpen,
        AwaitingParenthesisClosure,
        ParsingComplete
    };

    /// @brief Process a single character in attributes word (this is mostly to save repetitive coding).
    void ProcessAttribute(CRawEntry rawEntry,
                          bool negatingSymbolDetected,
                          AttributeDefinitionState& sectionToAssign,
                          std::vector<CViolation>& violations,
                          ViolationCode violationCode)
    {
        if (sectionToAssign != AttributeDefinitionState::Undefined)
        {
            CViolation detectedViolation({ rawEntry }, violationCode);
            violations.emplace_back(detectedViolation);
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
                              std::vector<CViolation>&  detectedViolations) noexcept
    {
        auto inversion = false;

        for (auto character: resolvedContet)
        {
            switch (character)
            {
                case 'R':
                case 'r':
                {
                    ProcessAttribute(rawEntry, inversion, readOnlySection, detectedViolations, ViolationCode::ReadAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'W':
                case 'w':
                {
                    ProcessAttribute(rawEntry, inversion, readWriteSection, detectedViolations, ViolationCode::ReadWriteAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'A':
                case 'a':
                {
                    ProcessAttribute(rawEntry, inversion, allocatableSection, detectedViolations, ViolationCode::AlloctableAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'X':
                case 'x':
                {
                    ProcessAttribute(rawEntry, inversion, executableSection, detectedViolations, ViolationCode::ExecutableAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case 'I':
                case 'i':
                case 'L':
                case 'l':
                {
                    ProcessAttribute(rawEntry, inversion, initializedSection, detectedViolations, ViolationCode::InitializedAttributeIsAlreadySet);
                    inversion = false;
                    break;
                }

                case '!':
                {
                    if (inversion)
                    {
                        CViolation detectedViolation({ rawEntry }, ViolationCode::NegatingSymbolIsAlreadySet);
                        detectedViolations.emplace_back(detectedViolation);
                        break;
                    }

                    inversion = true;
                    break;
                }

                default:
                {
                    CViolation detectedViolation({ rawEntry }, ViolationCode::UnrecognizedAttributeSymbolWasFound);
                    detectedViolations.emplace_back(detectedViolation);
                }
            }
        }
    }
}


std::shared_ptr<CMemoryStatementAttribute> CMemoryStatementAttributeParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    if (iterator->EntryType() != RawEntryType::ParenthesisOpen)
    {
        throw CMasterParsingException(
                MasterParsingExceptionType::InternalParserError,
                "'CMemoryStatementAttributeParser::TryParse' can only be called with 'iterator' pointing to a 'Parenthesis-Open'");
    }

    auto parserState = ParserState::AwaitingParenthesisOpen;
    auto doNotAdvance = false;

    CRawEntry parenthesisOpen;
    CRawEntry parenthesisClose;
    AttributeDefinitionState readOnlySection;     // Value: R
    AttributeDefinitionState readWriteSection;    // Value: W
    AttributeDefinitionState allocatableSection;  // Value: A
    AttributeDefinitionState executableSection;   // Value: X
    AttributeDefinitionState initializedSection;  // Value: I or L

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
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
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Word:
            {                
                switch (parserState)
                {
                    case ParserState::AwaitingParenthesisOpen:
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
                    case ParserState::AwaitingParenthesisOpen:
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
                localIterator = previousPositionIterator;
                parserState = ParserState::ParsingComplete;
                break;
            }

            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:            
            case RawEntryType::Unknown:
            {
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
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

        localIterator = ((parserState != ParserState::ParsingComplete) && !doNotAdvance) ?
                        localIterator + 1 :
                        localIterator;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

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
