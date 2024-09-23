#include "EDrcViolationCode.h"

#include <stdexcept>
#include <string>

using namespace VisualLinkerScript::DrcEngine;


std::string VisualLinkerScript::DrcEngine::MapDrcViolationToDescription(EDrcViolationCode code)
{
	switch (code)
	{
		case EDrcViolationCode::DuplicateNameForMemoryStatement:	return "Duplicate name for memory statement found";
		case EDrcViolationCode::IncludedFilesArePresentRule:		return "Included files are present violation";
		case EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce:	return "Entry directive defined more than once";
		case EDrcViolationCode::MemoryRegionDefinedMultipleTimes:	return "Memory region defined multiple times";
		case EDrcViolationCode::RecursiveIncludeChainDetected:		return "Recursive include chain detected";
		case EDrcViolationCode::SectionsRegionDefinedMultipleTimes: return "Sections region defined multiple times";
		case EDrcViolationCode::WasExpectingProgramHeaderTypeHere:	return "Was expecting program header typed here";
		default: throw std::invalid_argument("Code value not recognized");
	}
}

std::string VisualLinkerScript::DrcEngine::MapDrcViolationToCode(EDrcViolationCode code)
{
	switch (code)
	{
		case EDrcViolationCode::DuplicateNameForMemoryStatement:	return "D0001";
		case EDrcViolationCode::IncludedFilesArePresentRule:		return "D0002";
		case EDrcViolationCode::EntryDirectiveDefinedMoreThanOnce:	return "D0003";
		case EDrcViolationCode::MemoryRegionDefinedMultipleTimes:	return "D0004";
		case EDrcViolationCode::RecursiveIncludeChainDetected:		return "D0005";
		case EDrcViolationCode::SectionsRegionDefinedMultipleTimes: return "D0006";
		case EDrcViolationCode::WasExpectingProgramHeaderTypeHere:	return "D0007";
		default: throw std::invalid_argument("Code value not recognized");
	}
}
