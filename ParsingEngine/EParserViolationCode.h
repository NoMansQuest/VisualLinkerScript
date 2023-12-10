#ifndef EPARSERVIOLATIONCODE_H__
#define EPARSERVIOLATIONCODE_H__

#include <cstdint>

namespace VisualLinkerScript::ParsingEngine
{
    /// @brief An exhaustive list of all known violations.
    enum class EParserViolationCode : uint32_t
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
        NoSymbolOrKeywordAllowedAfterMemoryHeader,
        ReadAttributeIsAlreadySet,
        WasExpectingEqualOperatorHere,
        ReadWriteAttributeIsAlreadySet,
        AlloctableAttributeIsAlreadySet,
        CompoundAssignmentOperatoreNotAllowedHere,
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
        UnexpectedEarlyTermination,
        MissingLValue,

        // Assignment procedure parsing violations:
        WasExpectingSemicolonHere,        

        // Section Output Content
        OnlyOneSectionOutputTypeIsAllowed,
        SectionOutputNameCannotBeAReservedKeyword,
        MissingRegionForAtLmaDefinition,
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

        // Master Parser Errors
        MemoryRegionParsingFailed,
        SectionsRegionParsingFailed,
        PhdrsRegionParsingFailed,
        VersionRegionParsingFailed,

        // General violations:
        FunctionMissingDefinitionOrInvalidSymbolName,
        MissingOpeningCurlyBracket,
        MissingClosingCurlyBracket,
        MissingOpeningParenthesis,
        MissingClosingParenthesis,
        EntryInvalidOrMisplaced,
        WildcardsNotAllowedHere,
        DesignRuleCheckError // All DRC-Errors have this violation code.
    };
};

#endif //
