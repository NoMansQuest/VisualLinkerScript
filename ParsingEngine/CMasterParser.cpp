#include "CMasterParser.h"
#include "Models/CComment.h"
#include <algorithm>

#include "CMasterParserException.h"
#include "SubParsers/CAssignmentParser.h"
#include "SubParsers/CMemoryRegionParser.h"
#include "SubParsers/CFunctionParser.h"
#include "SubParsers/CPhdrsRegionParser.h"
#include "SubParsers/CSectionsRegionParser.h"
#include "SubParsers/CSubParserBase.h"
#include "SubParsers/Constants.h"
#include "SubParsers/CVersionRegionParser.h"
#include "SubParsers/CSectionOverlayParser.h"

#include "Models/CLinkerScriptContentBase.h"
#include "Models/Raw/RawEntryType.h"

#include <vector>
#include <memory>

using namespace VisualLinkerScript::ParsingEngine;
using namespace VisualLinkerScript::ParsingEngine::Models::Raw;
using namespace VisualLinkerScript::ParsingEngine::SubParsers;

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
    std::vector<CViolation> violations;

    while (localIterator != endOfVectorIterator)
    {
        auto resolvedContent = rawFile->ResolveRawEntry(*localIterator);

        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Word:
            {
                if (CParserHelpers::StringCompare(resolvedContent, "MEMORY", false))
                {

                }
                else if (CParserHelpers::StringCompare(resolvedContent, "SECTIONS", false))
                {

                }
                else if (CParserHelpers::StringCompare(resolvedContent, "VERSION", false))
                {

                }
                else
                {
                    // These would be special function calls
                    if ()

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
                violations.emplace_back(CViolation(*localIterator, ViolationCode::EntryInvalidOrMisplaced));
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
