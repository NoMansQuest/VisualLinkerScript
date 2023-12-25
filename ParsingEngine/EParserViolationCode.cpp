#include "EParserViolationCode.h"

using namespace VisualLinkerScript::ParsingEngine;

std::string VisualLinkerScript::ParsingEngine::MapParserViolationCodeToString(EParserViolationCode code)
{
    switch (code)
    {
        case EParserViolationCode::NoSymbolOrKeywordAllowedAfterPhdrsHeader: return "NoSymbolOrKeywordAllowedAfterPhdrsHeader";
        case EParserViolationCode::ProgramHeaderNameShouldNotBeAReservedKeyword: return "ProgramHeaderNameShouldNotBeAReservedKeyword";
        case EParserViolationCode::ProgramHeaderTypeNotRecognized:return "ProgramHeaderTypeNotRecognized";
        case EParserViolationCode::ProgramHeaderFileHdrAlreadySet:return "ProgramHeaderFileHdrAlreadySet";
        case EParserViolationCode::ProgramHeaderPhdrsAlreadySet:return "ProgramHeaderPhdrsAlreadySet";
        case EParserViolationCode::ProgramHeaderAtAddressAlreadySet:return "ProgramHeaderAtAddressAlreadySet";
        case EParserViolationCode::ProgramHeaderFlagsAlreadySet:return "ProgramHeaderFlagsAlreadySet";
        case EParserViolationCode::WasExpectingProgramHeaderTypeHere:return "WasExpectingProgramHeaderTypeHere";
        case EParserViolationCode::NoSymbolOrKeywordAllowedAfterMemoryHeader: return "NoSymbolOrKeywordAllowedAfterMemoryHeader";
        case EParserViolationCode::ReadAttributeIsAlreadySet: return "ReadAttributeIsAlreadySet";
        case EParserViolationCode::WasExpectingEqualOperatorHere: return "WasExpectingEqualOperatorHere";
        case EParserViolationCode::ReadWriteAttributeIsAlreadySet: return "ReadWriteAttributeIsAlreadySet";
        case EParserViolationCode::AlloctableAttributeIsAlreadySet: return "AlloctableAttributeIsAlreadySet";
        case EParserViolationCode::CompoundAssignmentOperatoreNotAllowedHere: return "CompoundAssignmentOperatoreNotAllowedHere";
        case EParserViolationCode::ExecutableAttributeIsAlreadySet: return "ExecutableAttributeIsAlreadySet";
        case EParserViolationCode::InitializedAttributeIsAlreadySet: return "InitializedAttributeIsAlreadySet";
        case EParserViolationCode::NegatingSymbolIsAlreadySet: return "NegatingSymbolIsAlreadySet";
        case EParserViolationCode::UnrecognizedAttributeSymbolWasFound: return "UnrecognizedAttributeSymbolWasFound";
        case EParserViolationCode::MemorySectionNameShouldNotBeAReservedKeyword: return "MemorySectionNameShouldNotBeAReservedKeyword";
        case EParserViolationCode::WasExpectingOriginDeclaration: return "WasExpectingOriginDeclaration";
        case EParserViolationCode::WasExpectingLengthDeclaration: return "WasExpectingLengthDeclaration";
        case EParserViolationCode::WasExpectingAttributeHere: return "WasExpectingAttributeHere";
        case EParserViolationCode::OperatorIsNotAcceptedHere: return "OperatorIsNotAcceptedHere";
        case EParserViolationCode::NoCorrespondingParenthesisOvertureFound: return "NoCorrespondingParenthesisOvertureFound";
        case EParserViolationCode::ExpectedOperatorAfterThisEntry: return "ExpectedOperatorAfterThisEntry";
        case EParserViolationCode::WasExpectingAnOperatorHere: return "WasExpectingAnOperatorHere";
        case EParserViolationCode::AssignmentOperatorNotValidInAnRValueExpression: return "AssignmentOperatorNotValidInAnRValueExpression";
        case EParserViolationCode::MissingOperatorInRValueExpression: return "MissingOperatorInRValueExpression";
        case EParserViolationCode::StringsNotAllowedInExpressions: return "StringsNotAllowedInExpressions";
        case EParserViolationCode::WasExpectingASymbolOrNumberBeforeExpressionEnds: return "WasExpectingASymbolOrNumberBeforeExpressionEnds";
        case EParserViolationCode::WasExpectingASymbolOrNumberHere: return "WasExpectingASymbolOrNumberHere";
        case EParserViolationCode::TernaryExpressionMissingFirstCondition: return "TernaryExpressionMissingFirstCondition";
        case EParserViolationCode::TernaryExpressionMissingElseOutcome: return "TernaryExpressionMissingElseOutcome";
        case EParserViolationCode::UnexpectedTerminationOfExpression: return "UnexpectedTerminationOfExpression";
        case EParserViolationCode::WasExpectingParenthesisOverture: return "WasExpectingParenthesisOverture";
        case EParserViolationCode::WasExpectingParenthesisClosure: return "WasExpectingParenthesisClosure";
        case EParserViolationCode::WasExpectingAnotherParameterOrParenthesisClosure: return "WasExpectingAnotherParameterOrParenthesisClosure";
        case EParserViolationCode::FunctionsCannotSpanMultipleLines: return "FunctionsCannotSpanMultipleLines";
        case EParserViolationCode::FunctionNotRecognized: return "FunctionNotRecognized";
        case EParserViolationCode::WasExpectingFirstParameterHere: return "WasExpectingFirstParameterHere";
        case EParserViolationCode::NoParameterProvidedForTheFunction: return "NoParameterProvidedForTheFunction";
        case EParserViolationCode::WasExpectingFunctioNameHere: return "WasExpectingFunctioNameHere";
        case EParserViolationCode::WasExpectingNextParameter: return "WasExpectingNextParameter";
        case EParserViolationCode::UnexpectedTerminationOfFunction: return "UnexpectedTerminationOfFunction";
        case EParserViolationCode::ConsecutiveParameterSeparator: return "ConsecutiveParameterSeparator";
        case EParserViolationCode::SeparatorExpectedInbetweenParameters: return "SeparatorExpectedInbetweenParameters";
        case EParserViolationCode::InputSectionFunctionSortParser: return "InputSectionFunctionSortParser";
        case EParserViolationCode::FunctionDoesNotSupportMultipleParameters: return "FunctionDoesNotSupportMultipleParameters";
        case EParserViolationCode::LValueCannotContainRValueExpression: return "LValueCannotContainRValueExpression";
        case EParserViolationCode::MultipleAssignmentOperatorsDetected: return "MultipleAssignmentOperatorsDetected";
        case EParserViolationCode::RValueExpressionParsingFailed: return "RValueExpressionParsingFailed";
        case EParserViolationCode::UnexpectedEarlyTermination: return "UnexpectedEarlyTermination";
        case EParserViolationCode::MissingLValue: return "MissingLValue";
        case EParserViolationCode::WasExpectingSemicolonHere: return "WasExpectingSemicolonHere";
        case EParserViolationCode::OnlyOneSectionOutputTypeIsAllowed: return "OnlyOneSectionOutputTypeIsAllowed";
        case EParserViolationCode::SectionOutputNameCannotBeAReservedKeyword: return "SectionOutputNameCannotBeAReservedKeyword";
        case EParserViolationCode::MissingRegionForAtLmaDefinition: return "MissingRegionForAtLmaDefinition";
        case EParserViolationCode::WasExpectingANumericValueForFillExpression: return "WasExpectingANumericValueForFillExpression";
        case EParserViolationCode::WasExpectingProgramHeaderName: return "WasExpectingProgramHeaderName";
        case EParserViolationCode::ProgramHeaderNameCannotBeAReservedWord: return "ProgramHeaderNameCannotBeAReservedWord";
        case EParserViolationCode::VmaRegionNameCannotBeAReservedWord: return "VmaRegionNameCannotBeAReservedWord";
        case EParserViolationCode::EntryNotAllowedInInputSectionDefinition: return "EntryNotAllowedInInputSectionDefinition";
        case EParserViolationCode::InvalidSyntaxInSectionInputFunctionDeclaration: return "InvalidSyntaxInSectionInputFunctionDeclaration";
        case EParserViolationCode::OperatorsNotAllowedWithinInputSectionFunctions: return "OperatorsNotAllowedWithinInputSectionFunctions";
        case EParserViolationCode::MissingParenthesisOvertureForFunction: return "MissingParenthesisOvertureForFunction";
        case EParserViolationCode::InputSectionFunctionParsingFailed: return "InputSectionFunctionParsingFailed";
        case EParserViolationCode::ReservedWordsNotAllowedHere: return "ReservedWordsNotAllowedHere";
        case EParserViolationCode::MissingFunctionBody: return "MissingFunctionBody";
        case EParserViolationCode::IllegalFunctionCallInSectionOutputStatement: return "IllegalFunctionCallInSectionOutputStatement";
        case EParserViolationCode::InvalidExcludeFileCall: return "InvalidExcludeFileCall";
        case EParserViolationCode::InvalidSortFunctionCall: return "InvalidSortFunctionCall";
        case EParserViolationCode::NotAllowedInOverlayCommand: return "NotAllowedInOverlayCommand";
        case EParserViolationCode::NoCrossRefsAlreadyDeclared: return "NoCrossRefsAlreadyDeclared";
        case EParserViolationCode::AtAddressAlreadyDeclared: return "AtAddressAlreadyDeclared";
        case EParserViolationCode::StartAddressExpressionAlreadyDeclared: return "StartAddressExpressionAlreadyDeclared";
        case EParserViolationCode::UnableToUnderstandOverlaySection: return "UnableToUnderstandOverlaySection";
        case EParserViolationCode::WasExpectingBracketOverture: return "WasExpectingBracketOverture";
        case EParserViolationCode::InvalidParentScope: return "InvalidParentScope";
        case EParserViolationCode::ParentScopeIsAlreadyDefined: return "ParentScopeIsAlreadyDefined";
        case EParserViolationCode::MemoryRegionParsingFailed: return "MemoryRegionParsingFailed";
        case EParserViolationCode::SectionsRegionParsingFailed: return "SectionsRegionParsingFailed";
        case EParserViolationCode::PhdrsRegionParsingFailed: return "PhdrsRegionParsingFailed";
        case EParserViolationCode::VersionRegionParsingFailed: return "VersionRegionParsingFailed";
        case EParserViolationCode::FunctionMissingDefinitionOrInvalidSymbolName: return "FunctionMissingDefinitionOrInvalidSymbolName";
        case EParserViolationCode::MissingOpeningCurlyBracket: return "MissingOpeningCurlyBracket";
        case EParserViolationCode::MissingClosingCurlyBracket: return "MissingClosingCurlyBracket";
        case EParserViolationCode::MissingOpeningParenthesis: return "MissingOpeningParenthesis";
        case EParserViolationCode::MissingClosingParenthesis: return "MissingClosingParenthesis";
        case EParserViolationCode::EntryInvalidOrMisplaced: return "EntryInvalidOrMisplaced";
        case EParserViolationCode::WildcardsNotAllowedHere: return "WildcardsNotAllowedHere";
        case EParserViolationCode::DesignRuleCheckError: return "DesignRuleCheckError";

        default:
            throw std::invalid_argument(NAMEOF(code));
    }
}
