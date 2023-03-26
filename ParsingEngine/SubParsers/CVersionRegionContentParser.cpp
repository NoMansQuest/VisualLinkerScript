#include <vector>
#include <memory>
#include <string>
#include "CVersionRegionContentParser.h"
#include "Constants.h"
#include "../CMasterParserException.h"
#include "../Models/CLinkerScriptContentBase.h"
#include "../Models/CVersionScope.h"
#include "../Models/CVersionTag.h"
#include "../Models/CVersionNode.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/Raw/RawEntryType.h"
#include "../Models/Raw/CRawFile.h"
#include "../Models/CComment.h"
#include "../Models/CViolation.h"

using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

namespace
{
    /// @brief Parse states for Sections Region Content Parser
    enum class ParserState
    {
        AwaitingHeader,
        AwaitingBracketOpen,
        AwaitingBracketClosure,
        AwaitingSemicolon,
        ParsingComplete
    };
}

std::shared_ptr<CVersionScope> CVersionRegionContentParser::TryParse(
        CRawFile& linkerScriptFile,
        std::vector<CRawEntry>::const_iterator& iterator,
        std::vector<CRawEntry>::const_iterator& endOfVectorIterator)
{
    std::vector<CRawEntry>::const_iterator localIterator = iterator;
    std::vector<CRawEntry>::const_iterator previousPositionIterator = iterator;
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = iterator;
    std::vector<CViolation> violations;

    auto parserState = ParserState::AwaitingHeader;
    auto doNotAdvance = false;

    CRawEntry currentVersionTag; // Set to 'Not Present' by default

    CRawEntry scopeHeaderEntry;
    CRawEntry bracketOpenEntry;
    CRawEntry bracketCloseEntry;
    CRawEntry parentScopeEntry;
    CRawEntry semicolonEntry;
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;

    while ((localIterator != endOfVectorIterator) && (parserState != ParserState::ParsingComplete))
    {
        doNotAdvance = false;

        auto resolvedContent = linkerScriptFile.ResolveRawEntry(*localIterator);
        auto localIteratorPlusOne = localIterator + 1;
        CRawEntry rawEntryPlusOne;
        std::string resolvedContentPlusOne;

        if (localIteratorPlusOne != endOfVectorIterator)
        {
            rawEntryPlusOne = *localIteratorPlusOne;
            resolvedContentPlusOne = linkerScriptFile.ResolveRawEntry(rawEntryPlusOne);
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
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            return nullptr;
                        }

                        scopeHeaderEntry = *localIterator;
                        parserState = ParserState::AwaitingBracketOpen;
                        break;
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::WasExpectingBracketOverture));
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        }
                        else if (resolvedContent.size() > 0 && resolvedContent[resolvedContent.size() - 1] == ':')
                        {
                            std::shared_ptr<CVersionTag> versionScope(new CVersionTag(*localIterator, {*localIterator}, {}));
                            currentVersionTag = *localIterator;
                            parsedContent.emplace_back(versionScope);
                        }
                        else
                        {
                            if (resolvedContentPlusOne == ";")
                            {
                                std::shared_ptr<CVersionNode> versionScope(
                                            new CVersionNode(
                                                *localIterator,
                                                rawEntryPlusOne,
                                                currentVersionTag,
                                                {*localIterator, rawEntryPlusOne},
                                                {}));
                                parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(versionScope));
                                localIterator++; // We advance by one, as we've alredy consume the successor entry.
                            }
                            else
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                            }
                        }

                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        if (CParserHelpers::IsReservedWord(resolvedContent))
                        {
                            violations.emplace_back(CViolation(*localIterator, ViolationCode::InvalidParentScope));
                        }
                        else
                        {
                            if (parentScopeEntry.IsPresent())
                            {
                                violations.emplace_back(CViolation(*localIterator, ViolationCode::ParentScopeIsAlreadyDefined));
                            }
                            else
                            {
                                parentScopeEntry = *localIterator;
                            }
                        }
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CVersionRegionContentParser");
                }

               break;
            }

            case RawEntryType::Operator:
            {
                switch (parserState)
                {
                    case ParserState::AwaitingHeader:
                    case ParserState::AwaitingBracketOpen:
                    {
                        return nullptr; // Grounds to abort
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    case ParserState::AwaitingSemicolon:
                    {
                        semicolonEntry = *localIterator;
                        parserState = ParserState::ParsingComplete;
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CVersionRegionContentParser");
                }

                break;
            }

            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::ParenthesisOpen:
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
                        return nullptr; // Grounds to abort
                    }

                    case ParserState::AwaitingBracketOpen:
                    {
                        bracketOpenEntry = *localIterator;
                        parserState = ParserState::AwaitingBracketClosure;
                        break;
                    }

                    case ParserState::AwaitingBracketClosure:
                    case ParserState::AwaitingSemicolon:
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CVersionRegionContentParser");
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
                        return nullptr; // Grounds to abort
                    }

                    case ParserState::AwaitingBracketClosure:
                    {
                        bracketOpenEntry = *localIterator;
                        parserState = ParserState::AwaitingSemicolon;
                        break;
                    }

                    case ParserState::AwaitingSemicolon :
                    {
                        violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
                        break;
                    }

                    default:
                        throw CMasterParsingException(
                                    MasterParsingExceptionType::ParserMachineStateNotExpectedOrUnknown,
                                    "ParserState invalid in CVersionRegionContentParser");
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
    std::copy(parsingStartIteratorPosition, localIterator, rawEntries.begin());

    iterator = localIterator;

    return std::shared_ptr<CVersionScope>(
                new CVersionScope(scopeHeaderEntry,
                                  parentScopeEntry,
                                  bracketOpenEntry,
                                  bracketCloseEntry,
                                  semicolonEntry,
                                  std::move(parsedContent),
                                  std::move(rawEntries),
                                  std::move(violations)));
}
