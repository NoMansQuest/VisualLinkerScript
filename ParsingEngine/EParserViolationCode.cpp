#include "EParserViolationCode.h"

using namespace VisualLinkerScript::ParsingEngine;

std::string VisualLinkerScript::ParsingEngine::MapParserViolationToDescription(EParserViolationCode code)
{
    switch (code)
    {
    case EParserViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader: return "No symbol or keyword allowed after PHDRs header";
    case EParserViolationCode::ProgramHeaderNameShouldNotBeAReservedKeyword: return "Program header name should not be a reserved keyword";
    case EParserViolationCode::ProgramHeaderTypeNotRecognized: return "Program header type not recognized";
    case EParserViolationCode::ProgramHeaderFileHdrAlreadySet: return "Program header file header already set";
    case EParserViolationCode::ProgramHeaderPhdrsAlreadySet: return "Program header PHDRs already set";
    case EParserViolationCode::ProgramHeaderAtAddressAlreadySet: return "Program header at address already set";
    case EParserViolationCode::ProgramHeaderFlagsAlreadySet: return "Program header flags already set";
    case EParserViolationCode::WasExpectingProgramHeaderTypeHere: return "Was expecting program header type here";
    case EParserViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader: return "No symbol or keyword allowed after memory header";
    case EParserViolationCode::IncompleteMemoryDefinition: return "Incomplete memory definition";
    case EParserViolationCode::ReadAttributeIsAlreadySet: return "Read attribute is already set";
    case EParserViolationCode::WasExpectingEqualOperatorHere: return "Was expecting equal operator here";
    case EParserViolationCode::ReadWriteAttributeIsAlreadySet: return "Read write attribute is already set";
    case EParserViolationCode::AllocatableAttributeIsAlreadySet: return "Allocatable attribute is already set";
    case EParserViolationCode::CompoundAssignmentOperatorNotAllowedHere: return "Compound assignment operator not allowed here";
    case EParserViolationCode::ExecutableAttributeIsAlreadySet: return "Executable attribute is already set";
    case EParserViolationCode::InitializedAttributeIsAlreadySet: return "Initialized attribute is already set";
    case EParserViolationCode::NegatingSymbolIsAlreadySet: return "Negating symbol is already set";
    case EParserViolationCode::UnrecognizedAttributeSymbolWasFound: return "Unrecognized attribute symbol was found";
    case EParserViolationCode::MemorySectionNameShouldNotBeAReservedKeyword: return "Memory section name should not be a reserved keyword";
    case EParserViolationCode::WasExpectingOriginDeclaration: return "Was expecting origin declaration";
    case EParserViolationCode::WasExpectingLengthDeclaration: return "Was expecting length declaration";
    case EParserViolationCode::WasExpectingAttributeHere: return "Was expecting attribute here";
    case EParserViolationCode::OperatorIsNotAcceptedHere: return "Operator is not accepted here";
    case EParserViolationCode::NoCorrespondingParenthesisOvertureFound: return "No corresponding parenthesis overture found";
    case EParserViolationCode::ExpectedOperatorAfterThisEntry: return "Expected operator after this entry";
    case EParserViolationCode::WasExpectingAnOperatorHere: return "Was expecting an operator here";
    case EParserViolationCode::AssignmentOperatorNotValidInAnRValueExpression: return "Assignment operator not valid in an r value expression";
    case EParserViolationCode::MissingOperatorInRValueExpression: return "Missing operator in r value expression";
    case EParserViolationCode::StringsNotAllowedInExpressions: return "Strings not allowed in expressions";
    case EParserViolationCode::WasExpectingASymbolOrNumberBeforeExpressionEnds: return "Was expecting a symbol or number before expression ends";
    case EParserViolationCode::WasExpectingASymbolOrNumberHere: return "Was expecting a symbol or number here";
    case EParserViolationCode::TernaryExpressionMissingFirstCondition: return "Ternary expression missing first condition";
    case EParserViolationCode::TernaryExpressionMissingElseOutcome: return "Ternary expression missing else outcome";
    case EParserViolationCode::UnexpectedTerminationOfExpression: return "Unexpected termination of expression";
    case EParserViolationCode::WasExpectingParenthesisOverture: return "Was expecting parenthesis overture";
    case EParserViolationCode::WasExpectingParenthesisClosure: return "Was expecting parenthesis closure";
    case EParserViolationCode::WasExpectingAnotherParameterOrParenthesisClosure: return "Was expecting another parameter or parenthesis closure";
    case EParserViolationCode::ParenthesisClosureForExpressionIsMissing: return "Parenthesis closure for expression is missing";
    case EParserViolationCode::FunctionsCannotSpanMultipleLines: return "Functions cannot span multiple lines";
    case EParserViolationCode::FunctionNotRecognized: return "Function not recognized";
    case EParserViolationCode::WasExpectingFirstParameterHere: return "Was expecting first parameter here";
    case EParserViolationCode::NoParameterProvidedForTheFunction: return "No parameter provided for the function";
    case EParserViolationCode::WasExpectingFunctioNameHere: return "Was expecting function name here";
    case EParserViolationCode::WasExpectingNextParameter: return "Was expecting next parameter";
    case EParserViolationCode::UnexpectedTerminationOfFunction: return "Unexpected termination of function";
    case EParserViolationCode::ConsecutiveParameterSeparator: return "Consecutive parameter separator";
    case EParserViolationCode::SeparatorExpectedInbetweenParameters: return "Separator expected in between parameters";
    case EParserViolationCode::InputSectionFunctionSortParser: return "Input section function sort parser";
    case EParserViolationCode::FunctionDoesNotSupportMultipleParameters: return "Function does not support multiple parameters";
    case EParserViolationCode::LValueCannotContainRValueExpression: return "L-value cannot contain R-value expression";
    case EParserViolationCode::MultipleAssignmentOperatorsDetected: return "Multiple assignment operators detected";
    case EParserViolationCode::RValueExpressionParsingFailed: return "R value expression parsing failed";
    case EParserViolationCode::RValueExpressionMissing: return "R value expression missing";
    case EParserViolationCode::UnexpectedEarlyTermination: return "Unexpected early termination";
    case EParserViolationCode::MissingLValue: return "Missing l value";
    case EParserViolationCode::ExpectedSemicolonWasNotFound: return "Expected semicolon was not found";
    case EParserViolationCode::AssignmentIsMissingParenthesisClosure: return "Assignment is missing parenthesis closure";
    case EParserViolationCode::OnlyOneSectionOutputTypeIsAllowed: return "Only one section output type is allowed";
    case EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword: return "Section output name cannot be a reserved keyword";
    case EParserViolationCode::MissingRegionForAtLmaDefinition: return "Missing region for at lma definition";
    case EParserViolationCode::WasExpectingANumericValueForFillExpression: return "Was expecting a numeric value for fill expression";
    case EParserViolationCode::WasExpectingProgramHeaderName: return "Was expecting program header name";
    case EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord: return "Program header name cannot be a reserved word";
    case EParserViolationCode::VmaRegionNameCannotBeAReservedWord: return "Vma region name cannot be a reserved word";
    case EParserViolationCode::EntryNotAllowedInInputSectionDefinition: return "Entry not allowed in input section definition";
    case EParserViolationCode::InvalidSyntaxInSectionInputFunctionDeclaration: return "Invalid syntax in section input function declaration";
    case EParserViolationCode::OperatorsNotAllowedWithinInputSectionFunctions: return "Operators not allowed within input section functions";
    case EParserViolationCode::MissingParenthesisOvertureForFunction: return "Missing parenthesis overture for function";
    case EParserViolationCode::InputSectionFunctionParsingFailed: return "Input section function parsing failed";
    case EParserViolationCode::ReservedWordsNotAllowedHere: return "Reserved words not allowed here";
    case EParserViolationCode::MissingFunctionBody: return "Missing function body";
    case EParserViolationCode::IllegalFunctionCallInSectionOutputStatement: return "Illegal function call in section output statement";
    case EParserViolationCode::InvalidExcludeFileCall: return "Invalid exclude file call";
    case EParserViolationCode::InvalidSortFunctionCall: return "Invalid sort function call";
    case EParserViolationCode::SectionOutputDefinitionIncomplete: return "Section output definition incomplete";
    case EParserViolationCode::SectionOutputBracketClosureMissing: return "Section output bracket closure missing";
    case EParserViolationCode::NotAllowedInOverlayCommand: return "Not allowed in overlay command";
    case EParserViolationCode::NoCrossRefsAlreadyDeclared: return "No cross refs already declared";
    case EParserViolationCode::AtAddressAlreadyDeclared: return "At address already declared";
    case EParserViolationCode::StartAddressExpressionAlreadyDeclared: return "Start address expression already declared";
    case EParserViolationCode::VersionScriptBracketClosureMissing: return "Version script bracket closure missing";
    case EParserViolationCode::VersionScriptSemicolonMissing: return "Version script semicolon missing";
    case EParserViolationCode::UnableToUnderstandOverlaySection: return "Unable to understand overlay section";
    case EParserViolationCode::WasExpectingBracketOverture: return "Was expecting bracket overture";
    case EParserViolationCode::InvalidParentScope: return "Invalid parent scope";
    case EParserViolationCode::ParentScopeIsAlreadyDefined: return "Parent scope is already defined";
    case EParserViolationCode::MemoryRegionParsingFailed: return "Memory region parsing failed";
    case EParserViolationCode::SectionsRegionParsingFailed: return "Sections region parsing failed";
    case EParserViolationCode::SectionsRegionBracketClosureMissing: return "Sections region bracket closure missing";
    case EParserViolationCode::PhdrsRegionParsingFailed: return "PHDRs region parsing failed";
    case EParserViolationCode::PhdrsCommandIsIncomplete: return "PHDRs command is incomplete";
    case EParserViolationCode::VersionRegionParsingFailed: return "Version region parsing failed";
    case EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName: return "Function missing definition or invalid symbol name";
    case EParserViolationCode::MissingOpeningCurlyBracket: return "Missing opening curly bracket";
    case EParserViolationCode::MissingClosingCurlyBracket: return "Missing closing curly bracket";
    case EParserViolationCode::MissingOpeningParenthesis: return "Missing opening parenthesis";
    case EParserViolationCode::MissingClosingParenthesis: return "Missing closing parenthesis";
    case EParserViolationCode::EntryInvalidOrMisplaced: return "Entry invalid or misplaced";
	case EParserViolationCode::EntryCanBeOmitted: return "Entry can be omitted";
    case EParserViolationCode::WildcardsNotAllowedHere: return "Wildcards not allowed here";
    case EParserViolationCode::DesignRuleCheckError: return "Design rule check error";

    default:
        throw std::invalid_argument(NAMEOF(code));
    }
}

std::string VisualLinkerScript::ParsingEngine::MapParserViolationToCode(EParserViolationCode code)
{
    switch (code)
    {
    case EParserViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader: return "P0001";
    case EParserViolationCode::ProgramHeaderNameShouldNotBeAReservedKeyword: return "P0002";
    case EParserViolationCode::ProgramHeaderTypeNotRecognized: return "P0003";
    case EParserViolationCode::ProgramHeaderFileHdrAlreadySet: return "P0004";
    case EParserViolationCode::ProgramHeaderPhdrsAlreadySet: return "P0005";
    case EParserViolationCode::ProgramHeaderAtAddressAlreadySet: return "P0006";
    case EParserViolationCode::ProgramHeaderFlagsAlreadySet: return "P0007";
    case EParserViolationCode::WasExpectingProgramHeaderTypeHere: return "P0008";
    case EParserViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader: return "P0009";
    case EParserViolationCode::IncompleteMemoryDefinition: return "P0010";
    case EParserViolationCode::ReadAttributeIsAlreadySet: return "P0011";
    case EParserViolationCode::WasExpectingEqualOperatorHere: return "P0012";
    case EParserViolationCode::ReadWriteAttributeIsAlreadySet: return "P0013";
    case EParserViolationCode::AllocatableAttributeIsAlreadySet: return "P0014";
    case EParserViolationCode::CompoundAssignmentOperatorNotAllowedHere: return "P0015";
    case EParserViolationCode::ExecutableAttributeIsAlreadySet: return "P0016";
    case EParserViolationCode::InitializedAttributeIsAlreadySet: return "P0017";
    case EParserViolationCode::NegatingSymbolIsAlreadySet: return "P0018";
    case EParserViolationCode::UnrecognizedAttributeSymbolWasFound: return "P0019";
    case EParserViolationCode::MemorySectionNameShouldNotBeAReservedKeyword: return "P0020";
    case EParserViolationCode::WasExpectingOriginDeclaration: return "P0021";
    case EParserViolationCode::WasExpectingLengthDeclaration: return "P0022";
    case EParserViolationCode::WasExpectingAttributeHere: return "P0023";
    case EParserViolationCode::OperatorIsNotAcceptedHere: return "P0024";
    case EParserViolationCode::NoCorrespondingParenthesisOvertureFound: return "P0025";
    case EParserViolationCode::ExpectedOperatorAfterThisEntry: return "P0026";
    case EParserViolationCode::WasExpectingAnOperatorHere: return "P0027";
    case EParserViolationCode::AssignmentOperatorNotValidInAnRValueExpression: return "P0028";
    case EParserViolationCode::MissingOperatorInRValueExpression: return "P0029";
    case EParserViolationCode::StringsNotAllowedInExpressions: return "P0030";
    case EParserViolationCode::WasExpectingASymbolOrNumberBeforeExpressionEnds: return "P0031";
    case EParserViolationCode::WasExpectingASymbolOrNumberHere: return "P0032";
    case EParserViolationCode::TernaryExpressionMissingFirstCondition: return "P0033";
    case EParserViolationCode::TernaryExpressionMissingElseOutcome: return "P0034";
    case EParserViolationCode::UnexpectedTerminationOfExpression: return "P0035";
    case EParserViolationCode::WasExpectingParenthesisOverture: return "P0036";
    case EParserViolationCode::WasExpectingParenthesisClosure: return "P0037";
    case EParserViolationCode::WasExpectingAnotherParameterOrParenthesisClosure: return "P0038";
    case EParserViolationCode::ParenthesisClosureForExpressionIsMissing: return "P0039";
    case EParserViolationCode::FunctionsCannotSpanMultipleLines: return "P0040";
    case EParserViolationCode::FunctionNotRecognized: return "P0041";
    case EParserViolationCode::WasExpectingFirstParameterHere: return "P0042";
    case EParserViolationCode::NoParameterProvidedForTheFunction: return "P0043";
    case EParserViolationCode::WasExpectingFunctioNameHere: return "P0044";
    case EParserViolationCode::WasExpectingNextParameter: return "P0045";
    case EParserViolationCode::UnexpectedTerminationOfFunction: return "P0046";
    case EParserViolationCode::ConsecutiveParameterSeparator: return "P0047";
    case EParserViolationCode::SeparatorExpectedInbetweenParameters: return "P0048";
    case EParserViolationCode::InputSectionFunctionSortParser: return "P0049";
    case EParserViolationCode::FunctionDoesNotSupportMultipleParameters: return "P0050";
    case EParserViolationCode::LValueCannotContainRValueExpression: return "P0051";
    case EParserViolationCode::MultipleAssignmentOperatorsDetected: return "P0052";
    case EParserViolationCode::RValueExpressionParsingFailed: return "P0053";
    case EParserViolationCode::RValueExpressionMissing: return "P0054";
    case EParserViolationCode::UnexpectedEarlyTermination: return "P0055";
    case EParserViolationCode::MissingLValue: return "P0056";
    case EParserViolationCode::ExpectedSemicolonWasNotFound: return "P0057";
    case EParserViolationCode::AssignmentIsMissingParenthesisClosure: return "P0058";
    case EParserViolationCode::OnlyOneSectionOutputTypeIsAllowed: return "P0059";
    case EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword: return "P0060";
    case EParserViolationCode::MissingRegionForAtLmaDefinition: return "P0061";
    case EParserViolationCode::WasExpectingANumericValueForFillExpression: return "P0062";
    case EParserViolationCode::WasExpectingProgramHeaderName: return "P0063";
    case EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord: return "P0064";
    case EParserViolationCode::VmaRegionNameCannotBeAReservedWord: return "P0065";
    case EParserViolationCode::EntryNotAllowedInInputSectionDefinition: return "P0066";
    case EParserViolationCode::InvalidSyntaxInSectionInputFunctionDeclaration: return "P0067";
    case EParserViolationCode::OperatorsNotAllowedWithinInputSectionFunctions: return "P0068";
    case EParserViolationCode::MissingParenthesisOvertureForFunction: return "P0069";
    case EParserViolationCode::InputSectionFunctionParsingFailed: return "P0070";
    case EParserViolationCode::ReservedWordsNotAllowedHere: return "P0071";
    case EParserViolationCode::MissingFunctionBody: return "P0072";
    case EParserViolationCode::IllegalFunctionCallInSectionOutputStatement: return "P0073";
    case EParserViolationCode::InvalidExcludeFileCall: return "P0074";
    case EParserViolationCode::InvalidSortFunctionCall: return "P0075";
    case EParserViolationCode::SectionOutputDefinitionIncomplete: return "P0076";
    case EParserViolationCode::SectionOutputBracketClosureMissing: return "P0077";
    case EParserViolationCode::NotAllowedInOverlayCommand: return "P0078";
    case EParserViolationCode::NoCrossRefsAlreadyDeclared: return "P0079";
    case EParserViolationCode::AtAddressAlreadyDeclared: return "P0080";
    case EParserViolationCode::StartAddressExpressionAlreadyDeclared: return "P0081";
    case EParserViolationCode::VersionScriptBracketClosureMissing: return "P0082";
    case EParserViolationCode::VersionScriptSemicolonMissing: return "P0083";
    case EParserViolationCode::UnableToUnderstandOverlaySection: return "P0084";
    case EParserViolationCode::WasExpectingBracketOverture: return "P0085";
    case EParserViolationCode::InvalidParentScope: return "P0086";
    case EParserViolationCode::ParentScopeIsAlreadyDefined: return "P0087";
    case EParserViolationCode::MemoryRegionParsingFailed: return "P0088";
    case EParserViolationCode::SectionsRegionParsingFailed: return "P0089";
    case EParserViolationCode::SectionsRegionBracketClosureMissing: return "P0090";
    case EParserViolationCode::PhdrsRegionParsingFailed: return "P0091";
    case EParserViolationCode::PhdrsCommandIsIncomplete: return "P0092";
    case EParserViolationCode::VersionRegionParsingFailed: return "P0093";
    case EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName: return "P0094";
    case EParserViolationCode::MissingOpeningCurlyBracket: return "P0095";
    case EParserViolationCode::MissingClosingCurlyBracket: return "P0096";
    case EParserViolationCode::MissingOpeningParenthesis: return "P0097";
    case EParserViolationCode::MissingClosingParenthesis: return "P0098";
    case EParserViolationCode::EntryInvalidOrMisplaced: return "P0099";
    case EParserViolationCode::EntryCanBeOmitted: return "P0100";
    case EParserViolationCode::WildcardsNotAllowedHere: return "P0101";
    case EParserViolationCode::DesignRuleCheckError: return "P0102";

    default:
        throw std::invalid_argument(NAMEOF(code));
    }
}
