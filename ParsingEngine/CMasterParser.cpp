
#include <vector>
#include <memory>
#include <algorithm>

#include "CMasterParserException.h"
#include "CMasterParser.h"

#include "SubParsers/CAssignmentParser.h"
#include "SubParsers/CAssignmentProcedureParser.h"
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

CAssignmentParser assignmentParser;
CAssignmentProcedureParser assignmentProcedureParser;
CPhdrsRegionParser phdrsRegionParser;
CMemoryParserRegion memoryParserRegion;
CSectionsRegionParser sectionsRegionParser;
CVersionRegionParser versionRegionParser;
CFunctionParser singleParameterFunctionParser;
CFunctionParser multiParameterFunctionParser(false, true);

bool TryParseLinkerScriptMasterBlocks(
        const std::string& resolvedContent,
        SharedPtrVector<CViolationBase>& violations,
        SharedPtrVector<CLinkerScriptContentBase>& parsedContent,
        std::vector<CRawEntry>::const_iterator& entryIterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator,
        std::shared_ptr<CRawFile> rawFile); // i.e. MEMORY, VERSION, ETC.

bool TryParseFunctionAndAssignmentProcedureCalls(
        const std::string& resolvedContent,
        SharedPtrVector<CViolationBase>& violations,
        SharedPtrVector<CLinkerScriptContentBase>& parsedContent,
        std::vector<CRawEntry>::const_iterator& entryIterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator,
        std::shared_ptr<CRawFile> rawFile); // ENTRY(....) or, PROVIDE(X=Y);, etc.

bool TryParseAssignments(
        const std::string& resolvedContent,
        SharedPtrVector<CViolationBase>& violations,
        SharedPtrVector<CLinkerScriptContentBase>& parsedContent,
        std::vector<CRawEntry>::const_iterator& entryIterator,
        const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
        std::shared_ptr<CRawFile> rawFile);

void CMasterParser::ParseLinkerScriptFile(std::shared_ptr<CLinkerScriptFile> linkerScriptFile)
{
    std::vector<CRawEntry>::const_iterator localIterator = linkerScriptFile->RawFile()->Content().cbegin();
    std::vector<CRawEntry>::const_iterator parsingStartIteratorPosition = linkerScriptFile->RawFile()->Content().cbegin();
    std::vector<CRawEntry>::const_iterator endOfVectorIterator = linkerScriptFile->RawFile()->Content().cend();
    SharedPtrVector<CLinkerScriptContentBase> parsedContent;
    SharedPtrVector<CViolationBase> violations;

    while (localIterator != endOfVectorIterator)
    {
        auto resolvedContent = linkerScriptFile->ResolveEntryText(*localIterator);
        switch (localIterator->EntryType())
        {
            case RawEntryType::Comment:
            {
                parsedContent.emplace_back(std::shared_ptr<CComment>(new CComment({*localIterator},{})));
                break;
            }

            case RawEntryType::Word:
            {
                if (TryParseLinkerScriptMasterBlocks(resolvedContent, violations, parsedContent, localIterator, endOfVectorIterator, linkerScriptFile->RawFile()))
                {
                    break;
                }
                else if (TryParseFunctionAndAssignmentProcedureCalls(resolvedContent, violations, parsedContent, localIterator, endOfVectorIterator, linkerScriptFile->RawFile()))
                {
                    break;
                }
                else if (TryParseAssignments(resolvedContent, violations, parsedContent, localIterator, endOfVectorIterator, linkerScriptFile->RawFile()))
                {
                    break;
                }
                else
                {
                    violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*localIterator, EParserViolationCode::EntryInvalidOrMisplaced)));
                }
                break;
            }

            case RawEntryType::ParenthesisOpen:
            case RawEntryType::ParenthesisClose:
            case RawEntryType::BracketOpen:
            case RawEntryType::BracketClose:
            case RawEntryType::ArithmeticOperator:
            case RawEntryType::AssignmentOperator:
            case RawEntryType::EvaluativeOperators:
            case RawEntryType::Colon:
            case RawEntryType::Comma:
            case RawEntryType::Semicolon:
            case RawEntryType::QuestionMark:
            case RawEntryType::Wildcard:
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

        // Can't advance an interator beyond 'endOfVector'.
        if (localIterator != endOfVectorIterator) {
            localIterator++;
        }
    }

    // And finally record our output...
    linkerScriptFile->UpdateParsedContent(parsedContent);
    linkerScriptFile->UpdateParserViolations(violations);
}

bool TryParseLinkerScriptMasterBlocks(
        const std::string& resolvedContent,
        SharedPtrVector<CViolationBase>& violations,
        SharedPtrVector<CLinkerScriptContentBase>& parsedContent,
        std::vector<CRawEntry>::const_iterator& entryIterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator,
        std::shared_ptr<CRawFile> rawFile)
{
    if (StringEquals(resolvedContent, "MEMORY", false))
    {
        auto parsedMemoryRegion = memoryParserRegion.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (parsedMemoryRegion == nullptr)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*entryIterator, EParserViolationCode::MemoryRegionParsingFailed)));
        }
        else
        {
            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedMemoryRegion));
        }
    }
    else if (StringEquals(resolvedContent, "SECTIONS", false))
    {
        auto parsedSectionsRegion = sectionsRegionParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (parsedSectionsRegion == nullptr)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*entryIterator, EParserViolationCode::SectionsRegionParsingFailed)));
        }
        else
        {
            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedSectionsRegion));
        }
    }
    else if (StringEquals(resolvedContent, "PHDRS", false))
    {
        auto parsedPhdrsRegion = phdrsRegionParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (parsedPhdrsRegion == nullptr)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*entryIterator, EParserViolationCode::PhdrsRegionParsingFailed)));
        }
        else
        {
            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedPhdrsRegion));
        }
    }
    else if (StringEquals(resolvedContent, "VERSION", false))
    {
        auto parsedVersionRegion = versionRegionParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (parsedVersionRegion == nullptr)
        {
            violations.emplace_back(std::shared_ptr<CViolationBase>(new CParserViolation(*entryIterator, EParserViolationCode::VersionRegionParsingFailed)));
        }
        else
        {
            parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedVersionRegion));
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool TryParseFunctionAndAssignmentProcedureCalls(
        const std::string& resolvedContent,
        SharedPtrVector<CViolationBase>& violations,
        SharedPtrVector<CLinkerScriptContentBase>& parsedContent,
        std::vector<CRawEntry>::const_iterator& entryIterator,
        std::vector<CRawEntry>::const_iterator endOfVectorIterator,
        std::shared_ptr<CRawFile> rawFile)
{
    CRawEntry rawEntryPlusOne;
    if (entryIterator + 1 != endOfVectorIterator)
    {
	    rawEntryPlusOne = *(entryIterator+1);
    }

    // These would be special function calls
    if (StringEquals(resolvedContent, "INCLUDE", false) && rawEntryPlusOne.IsPresent() && ((rawEntryPlusOne.EntryType() == RawEntryType::String) || (rawEntryPlusOne.EntryType() == RawEntryType::Word)))
    {
        auto parsedIncludeCommand = std::shared_ptr<CIncludeCommand>(new CIncludeCommand(*entryIterator, rawEntryPlusOne, {*entryIterator, rawEntryPlusOne},{}));
        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedIncludeCommand));
        ++entryIterator; // Manually advance the iterator by 1. The master loop will also advance it again, ensuring normal operation of the parser.
    }
    else if (StringIn(resolvedContent, {"INPUT", "GROUP", "AS_NEEDED"}, false))
    {
        auto parsedMultiParamFunctionCall = multiParameterFunctionParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (parsedMultiParamFunctionCall == nullptr)
        {
            return false;
        }

        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedMultiParamFunctionCall));
    }
    else if (StringIn(resolvedContent, {"OUTPUT", "SEARCH_DIR", "STARTUP"}, false) || CParserHelpers::IsFunctionName(resolvedContent))
    {
        auto parsedSingleParamFunctionCall = singleParameterFunctionParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (parsedSingleParamFunctionCall == nullptr)
        {
            return false;
        }

        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedSingleParamFunctionCall));
    }
    else if (CParserHelpers::IsAssignmentProcedure(resolvedContent))
    {
        auto assignmentProcedureStatement = assignmentProcedureParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
        if (assignmentProcedureStatement == nullptr)
        {
            return false;
        }

        parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(assignmentProcedureStatement));
    }
    else
    {
        return false;
    }

    return true;
}

bool TryParseAssignments(
        const std::string& resolvedContent,
        SharedPtrVector<CViolationBase>& violations,
        SharedPtrVector<CLinkerScriptContentBase>& parsedContent,
        std::vector<CRawEntry>::const_iterator& entryIterator,
        const std::vector<CRawEntry>::const_iterator& endOfVectorIterator,
        std::shared_ptr<CRawFile> rawFile)
{
    auto parsedAssignment = assignmentParser.TryParse(*rawFile, entryIterator, endOfVectorIterator);
    if (parsedAssignment == nullptr)
    {
        return false;
    }

    parsedContent.emplace_back(std::dynamic_pointer_cast<CLinkerScriptContentBase>(parsedAssignment));
    return true;
}

