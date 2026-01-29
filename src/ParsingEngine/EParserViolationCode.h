#ifndef EPARSERVIOLATIONCODE_H__
#define EPARSERVIOLATIONCODE_H__

#include <cstdint>
#include <stdexcept>
#include <string>

namespace VisualLinkerScript::ParsingEngine
{
    /// @brief An exhaustive list of all known violations.
    enum class EParserViolationCode: uint32_t
    {
        // Program Header (PHDRS) violations:
        NoSymbolOrKeywordAllowedAfterPhdrsHeader,
        ProgramHeaderNameShouldNotBeAReservedKeyword,
        ProgramHeaderTypeNotRecognized,
        ProgramHeaderFileHdrAlreadySet,
        ProgramHeaderPhdrsAlreadySet,
        ProgramHeaderAtAddressAlreadySet,
        ProgramHeaderFlagsAlreadySet,
        WasExpectingProgramHeaderTypeHere,

        // Memory region violations:
        IncompleteMemoryDefinition,
        NoSymbolOrKeywordAllowedAfterMemoryHeader,
        ReadAttributeIsAlreadySet,
        WasExpectingEqualOperatorHere,
        ReadWriteAttributeIsAlreadySet,
        AllocatableAttributeIsAlreadySet,
        CompoundAssignmentOperatorNotAllowedHere,
        ExecutableAttributeIsAlreadySet,
        InitializedAttributeIsAlreadySet,
        NegatingSymbolIsAlreadySet,
        UnrecognizedAttributeSymbolWasFound,
        MemorySectionNameShouldNotBeAReservedKeyword,
        WasExpectingOriginDeclaration,
        WasExpectingLengthDeclaration,
        WasExpectingAttributeHere,

        // Expression violations:
        OperatorIsNotAcceptedHere,
        NoCorrespondingParenthesisOvertureFound,
        ExpectedOperatorAfterThisEntry,
        WasExpectingAnOperatorHere,
        AssignmentOperatorNotValidInAnRValueExpression,
        MissingOperatorInRValueExpression,
        StringsNotAllowedInExpressions,
        WasExpectingASymbolOrNumberBeforeExpressionEnds,
        WasExpectingASymbolOrNumberHere,
        TernaryExpressionMissingFirstCondition,
        TernaryExpressionMissingElseOutcome,
        UnexpectedTerminationOfExpression,
        WasExpectingParenthesisOverture,
        WasExpectingParenthesisClosure,
        WasExpectingAnotherParameterOrParenthesisClosure,
        ParenthesisClosureForExpressionIsMissing,

        // Functions parsing violations:
        FunctionsCannotSpanMultipleLines,
        FunctionNotRecognized,
        WasExpectingFirstParameterHere,
        NoParameterProvidedForTheFunction,
        WasExpectingFunctioNameHere,
        WasExpectingNextParameter,
        UnexpectedTerminationOfFunction,
        ConsecutiveParameterSeparator,
        SeparatorExpectedInbetweenParameters,
        InputSectionFunctionSortParser,
        FunctionDoesNotSupportMultipleParameters,

        // Assignment parsing violations:
        LValueCannotContainRValueExpression,
        MultipleAssignmentOperatorsDetected,
        RValueExpressionParsingFailed,
        RValueExpressionMissing,
        UnexpectedEarlyTermination,
        MissingLValue,

        // Assignment procedure parsing violations:
        ExpectedSemicolonWasNotFound,
        AssignmentIsMissingParenthesisClosure,

        // Section Output Content
        OnlyOneSectionOutputTypeIsAllowed,
        SectionOutputNameCannotBeAReservedKeyword,
        MissingRegionForAtLmaDefinition,
        MissingRegionForAtVmaDefinition,
        WasExpectingANumericValueForFillExpression,
        WasExpectingProgramHeaderName,
        ProgramHeaderNameCannotBeAReservedWord,
        VmaRegionNameCannotBeAReservedWord,
        EntryNotAllowedInInputSectionDefinition,
        InvalidSyntaxInSectionInputFunctionDeclaration,
        OperatorsNotAllowedWithinInputSectionFunctions,
        MissingParenthesisOvertureForFunction,
        InputSectionFunctionParsingFailed,
        ReservedWordsNotAllowedHere,
        MissingFunctionBody,
        IllegalFunctionCallInSectionOutputStatement,
        InvalidExcludeFileCall,
        InvalidSortFunctionCall,
        SectionOutputDefinitionIncomplete,
        SectionOutputBracketClosureMissing,

        // OVERLAYS:
        NotAllowedInOverlayCommand,
        NoCrossRefsAlreadyDeclared,
        AtAddressAlreadyDeclared,
        StartAddressExpressionAlreadyDeclared,
        UnableToUnderstandOverlaySection,

        // VERSION:
        WasExpectingBracketOverture,
        InvalidParentScope,
        ParentScopeIsAlreadyDefined,
        VersionScriptBracketClosureMissing,
        VersionScriptSemicolonMissing,

        // Master Parser Errors
        MemoryRegionParsingFailed,
        SectionsRegionParsingFailed,
        SectionsRegionBracketClosureMissing,
        PhdrsRegionParsingFailed,
        PhdrsCommandIsIncomplete,
        VersionRegionParsingFailed,

        // General violations:
        FunctionMissingDefinitionOrInvalidSymbolName,
        MissingOpeningCurlyBracket,
        MissingClosingCurlyBracket,
        MissingOpeningParenthesis,
        MissingClosingParenthesis,
        EntryInvalidOrMisplaced,
        EntryCanBeOmitted,
        WildcardsNotAllowedHere,
        DesignRuleCheckError // All DRC-Errors have this violation code.
    };

    /// @brief Maps parser violation code to string.
    std::string MapParserViolationToDescription(EParserViolationCode code);

    /// @brief Maps parser violation to a unique ID code.
    std::string MapParserViolationToCode(EParserViolationCode code);
};

#endif //
