#include "CMemoryRegionContentParser.h"
#include <vector>
#include <memory>
#include <string>
#include "Constants.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../CMasterParserException.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CMemoryStatement.h"

using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

/*
MEMORY
{
    rom (rx)  : ORIGIN = 0, LENGTH = 256K
    ram (!rx) : org = 0x40000000, l = 4M
}
*/

namespace
{
    /// @brief CMemoryRegionContentParser parser states
    enum class ParserState
    {
        AwaitingName,
        AwaitingAttributes,
        AwaitingColon,
        AwaitingOriginAssignment,
        AwaitingLengthAssignment,
        AwaitingNewLineOrBracketClosure,
        ParsingComplete
    };
}

std::shared_ptr<CLinkerScriptContentBase> CMemoryRegionContentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    std::vector<CViolation> violations;

    auto parserState = ParserState::AwaitingName;
    auto doNotAdvance = false;

    CRawEntry nameEntry;
    std::shared_ptr<CLinkerScriptContentBase> attributes;
    CRawEntry colonEntry;
    std::shared_ptr<CLinkerScriptContentBase> originAssignment;
    std::shared_ptr<CLinkerScriptContentBase> lengthAssignment;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;
        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto lineChangeDetected = parsingStartIteratorPosition->EndLineNumber() != localIterator->EndLineNumber();

        if (lineChangeDetected)
        {
            break; // Any line-change would be rended parsing attempt null and void
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
                    case ParserState::AwaitingName:
                    {
                        if (ParserHelpers::IsReservedWord(resolvedContent))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::MemorySectionNameShouldNotBeAReservedKeyword));
                        }

                        nameEntry = *iterator;
                        parserState = ParserState::AwaitingAttributes;
                        break;
                    }

                    case ParserState::AwaitingAttributes:
                    {
                        if (!this->CheckIfRawEntryInList(linkerScriptFile, *localIterator, ListOfProgramHeaderTypes))
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingProgramHeaderTypeHere));
                            parserState = ParserState::AwaitingSemicolon;
                        }
                        else
                        {
                            typeEntry = *iterator;
                            parserState = ParserState::AwaitingOptionalFields;
                        }

                        break;
                    }

                    case ParserState::AwaitingColon:
                    {
                        if (resolvedContent == "FILEHDR")
                        {
                            if (fileHdrEntry.IsPresent())
                            {
                                CViolation detectedViolation({*localIterator, fileHdrEntry }, ViolationCode::ProgramHeaderFileHdrAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                fileHdrEntry = *localIterator;
                            }
                        }
                        else if (resolvedContent == "PHDRS")
                        {
                            if (phdrsEntry.IsPresent())
                            {
                                CViolation detectedViolation({ *localIterator, phdrsEntry }, ViolationCode::ProgramHeaderPhdrsAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                fileHdrEntry = *localIterator;
                            }
                        }
                        else if (resolvedContent == "AT")
                        {
                            if (atAddressFunction != nullptr)
                            {
                                CViolation detectedViolation({ *localIterator, phdrsEntry }, ViolationCode::ProgramHeaderAtAddressAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                auto parsedFunction = this->m_expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedFunction == nullptr)
                                {
                                    CViolation detectedViolation({ *localIterator, phdrsEntry }, ViolationCode::EntryInvalidOrMisplaced);
                                    violations.emplace_back(detectedViolation);
                                }
                                else
                                {
                                    atAddressFunction = parsedFunction;
                                }
                            }
                        }
                        else if (resolvedContent == "FLAGS")
                        {
                            if (atAddressFunction != nullptr)
                            {
                                CViolation detectedViolation({*localIterator, phdrsEntry}, ViolationCode::ProgramHeaderFlagsAlreadySet);
                                violations.emplace_back(detectedViolation);
                            }
                            else
                            {
                                auto parsedFunction = this->m_expressionParser.TryParse(linkerScriptFile, localIterator, endOfVectorIterator);
                                if (parsedFunction == nullptr)
                                {
                                    CViolation detectedViolation({*localIterator, phdrsEntry}, ViolationCode::EntryInvalidOrMisplaced);
                                    violations.emplace_back(detectedViolation);
                                }
                                else
                                {
                                    atAddressFunction = parsedFunction;
                                }
                            }
                        }
                        else
                        {
                            // We need to abort. Continue to semicolon to recover...
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::ProgramHeaderTypeNotRecognized));
                            parserState = ParserState::AwaitingSemicolon;
                        }

                        break;
                    }

                    case ParserState::AwaitingOriginAssignment:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        parserState = ParserState::AwaitingSemicolon;
                    }

                    case ParserState::AwaitingLengthAssignment:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        parserState = ParserState::AwaitingSemicolon;
                    }

                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                    }
                }
                break;
            }

            case RawEntryType::Operator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingAttributes:
                    {
                        if (resolvedContent[0] == '(')
                        {
                            auto attributeParsingResult = this->m_attributeParser.TryParse(linkerScriptFile, iterator, endOfVectorIterator);
                            if (attributeParsingResult == nullptr)
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::ProgramHeaderTypeNotRecognized));
                            }
                            else
                            {


                            }
                        }
                        break;
                    }

                    case ParserState::AwaitingName:
                    case ParserState::AwaitingColon:
                    case ParserState::AwaitingOriginAssignment:
                    case ParserState::AwaitingLengthAssignment:
                    default:
                    {
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CPhdrsRegionContentParser");
                    }
                }


                break;
            }

            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            {
                if (resolvedContent == ";")
                {
                    if (parserState == ParserState::AwaitingSemicolon)
                    {
                        semicolonEntry = *iterator;
                        break;
                    }
                    parserState = ParserState::ParsingComplete;
                }
                else
                {
                    violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                    parserState = ParserState::AwaitingSemicolon;
                }
                break;
            }

            case RawEntryType::Unknown:
            {
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'Unknown' entry was detected.");
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

    if (!nameEntry.IsPresent() || !colonEntry.IsPresent())
    {
        // It is safe to say that we have failed parsing this statement. It's malformed beyond recognition or
        // outright invalid.
        return nullptr;
    }

    std::vector<CRawEntry> rawEntries;
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    // We repot back the position parsing should continue with;
    iterator = (localIterator == endOfVectorIterator) ?
                localIterator :
                localIterator + 1;

    return std::shared_ptr<CLinkerScriptContentBase>(
                new CMemoryStatement(nameEntry,
                                     attributes,
                                     colonEntry,
                                     originAssignment,
                                     lengthAssignment,
                                     std::move(rawEntries),
                                     std::move(violations)));;
}
