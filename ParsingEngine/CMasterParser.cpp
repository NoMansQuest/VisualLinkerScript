
#include <vector>
#include <memory>
#include <algorithm>

#include "CMasterParserException.h"
#include "CMasterParser.h"

#include "SubParsers/CAssignmentParser.h"
#include "SubParsers/CMemoryRegionParser.h"
#include "SubParsers/CFunctionParser.h"
#include "SubParsers/CPhdrsRegionParser.h"
#include "SubParsers/CSectionsRegionParser.h"
#include "SubParsers/Constants.h"
#include "SubParsers/CVersionRegionParser.h"

#include "../Models/CComment.h"
#include "../Models/CIncludeCommand.h"
#include "../Models/CLinkerScriptContentBase.h"
#include "../Models/Raw/RawEntryType.h"

#include "../../../../Helpers.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;
using namespace VisualLinkerScript::Models::Raw;


std::shared_ptr<CLinkerScriptFile> CMasterParser::ProcessLinkerScriptFile(std::shared_ptr<CRawFile> rawFile)
{
    CAssignmentParser assignmentParser;
    CPhdrsRegionParser phdrsRegionParser;
    CMemoryParserRegion memoryParserRegion;
    CSectionsRegionParser sectionsRegionParser;
    CVersionRegionParser versionRegionParser;
    CFunctionParser singleParameterFunctionParser;
    CFunctionParser multiParameterFunctionParser(false, true);

    std::vector<CRawEntry>::const_iterator localIterator =  rawFile->Content().cbegin();
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition =  rawFile->Content().cbegin();
    std::vector<CRawEntry>::const_iterator endOfVectorIterator = rawFile->Content().cend();
    std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    while (localIterator != endOfVectorIterator)
    {
        auto resolvedContent = rawFile->ResolveRawEntry(*localIterator);
        CRawEntry rawEntryPlusOne;
        std::string resolvedContentPlusOne;
        if (localIterator + 1 != endOfVectorIterator)
        {
            rawEntryPlusOne = *(localIterator+1);
            resolvedContentPlusOne = rawFile->ResolveRawEntry(rawEntryPlusOne);
        }

        auto resolvedContentPl = rawFile->ResolveRawEntry(*localIterator);

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Word:
            {
                if (StringEquals(resolvedContent, "MEMORY", false))
                {
                    auto parsedMemoryRegion = memoryParserRegion.TryParse(*rawFile, localIterator, endOfVectorIterator);
                    if (parsedMemoryRegion == nullptr)
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::MemoryRegionParsingFailed)));
                    }
                    else
                    {
                        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedMemoryRegion));
                    }
                    break;
                }
                else if (StringEquals(resolvedContent, "SECTIONS", false))
                {
                    auto parsedSectionsRegion = sectionsRegionParser.TryParse(*rawFile, localIterator, endOfVectorIterator);
                    if (parsedSectionsRegion == nullptr)
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::SectionsRegionParsingFailed)));
                    }
                    else
                    {
                        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedSectionsRegion));
                    }
                    break;
                }
                else if (StringEquals(resolvedContent, "PHDRS", false))
                {
                    auto parsedPhdrsRegion = phdrsRegionParser.TryParse(*rawFile, localIterator, endOfVectorIterator);
                    if (parsedPhdrsRegion == nullptr)
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::PhdrsRegionParsingFailed)));
                    }
                    else
                    {
                        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedPhdrsRegion));
                    }
                    break;
                }
                else if (StringEquals(resolvedContent, "VERSION", false))
                {
                    auto parsedVersionRegion = versionRegionParser.TryParse(*rawFile, localIterator, endOfVectorIterator);
                    if (parsedVersionRegion == nullptr)
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::VersionRegionParsingFailed)));
                    }
                    else
                    {
                        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedVersionRegion));
                    }
                    break;
                }
                else
                {
                    // These would be special function calls
                    if (StringEquals(resolvedContent, "INCLUDE", false) &&
                        rawEntryPlusOne.IsPresent() &&
                        ((rawEntryPlusOne.EntryType() == RawEntryType::String) || (rawEntryPlusOne.EntryType() == RawEntryType::Word)))
                    {
                        auto parsedIncludeCommand = std::shared_ptr<CIncludeCommand>(
                                    new CIncludeCommand(*localIterator, rawEntryPlusOne, {*localIterator, rawEntryPlusOne},{}));
                        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedIncludeCommand));
                    }
                    else if (StringIn(resolvedContent, {"INPUT", "GROUP", "AS_NEEDED"}, false))
                    {
                        auto parsedMultiParamFunctionCall = multiParameterFunctionParser.TryParse(*rawFile, localIterator, endOfVectorIterator);
                        if (parsedMultiParamFunctionCall == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedMultiParamFunctionCall));
                        }
                    }
                    else if (StringIn(resolvedContent, {"OUTPUT","SEARCH_DIR", "STARTUP"}, false))
                    {
                        auto parsedSingleParamFunctionCall = singleParameterFunctionParser.TryParse(*rawFile, localIterator, endOfVectorIterator);
                        if (parsedSingleParamFunctionCall == nullptr)
                        {
                            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                        }
                        else
                        {
                            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedSingleParamFunctionCall));
                        }
                    }
                    else
                    {
                        violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                    }
                }

                break;
            }

            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            case RawEntryType::Operator:
            case RawEntryType::Assignment:
            case RawEntryType::Number:
            case RawEntryType::String:
            case RawEntryType::Unknown:
            {
                violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                break;
            }

            case RawEntryType::NotPresent:
                throw CMasterParsingException(
                        MasterParsingExceptionType::NotPresentEntryDetected,
                        "A 'non-present' entry was detected.");

            default:
                throw CMasterParsingException(
                        MasterParsingExceptionType::UnrecognizableRawEntryTypeValueFound,
                        "Unrecognized raw-entry type detected.");
        }

        localIterator++;
    }

    return std::make_shared<CLinkerScriptFile>(rawFile, std::move(parsedContent), std::move(violations));
}
