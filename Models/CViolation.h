#ifndef CVIOLATION_H__
#define CVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models 
{
    /// @brief An exhaustive list of all known violations.
    enum class ViolationCode : uint32_t
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
        ReadWriteAttributeIsAlreadySet,
        AlloctableAttributeIsAlreadySet,
        ExecutableAttributeIsAlreadySet,
        InitializedAttributeIsAlreadySet,
        NegatingSymbolIsAlreadySet,
        UnrecognizedAttributeSymbolWasFound,
        MemorySectionNameShouldNotBeAReservedKeyword,
        WasExpectingAttributeHereButEitherNotPresentOrInvalid,

        // Expression violations:
        OperatorIsNotAcceptedHere,
        NoCorrespondingParenthesisOvertureFound,
        AssignmentOperatorNotValidInAnRValueExpression,
        MissingOperatorInRValueExpression,
        WasExpectingASymbolOrNumberBeforeExpressionEnds,
        UnexpectedTerminationOfExpression,
        TernaryExpressionMissingFirstPart,
        WasExpectingParenthesisOverture,
        WasExpectingAnotherParameterOrParenthesisClosure,

        // Functiosn parsing violations:
        FunctionsCannotExpandToMultipleLines,
        FunctionNotRecognized,
        WasExpectingFunctioNameHere,
        UnexpectedTerminationOfFunction,

        // Assignment parsing violations:
        LValueCannotContainRValueExpression,
        MultipleAssignmentOperatorsDetected,
        RValueExpressionParsingFailed,
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
        EntryInvalidOrMisplaced
    };

    /// @brief This object contains information about a detected violation in the Linker Script
    class CViolation
    {
    private:
        CRawEntry m_entryBeforeViolation;
        CRawEntry m_entryAfterViolation;
        std::vector<CRawEntry> m_involvedEntries;        
        ViolationCode m_violationCode;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CViolation(std::vector<CRawEntry>&& involvedEntries, ViolationCode violationCode) 
            : m_involvedEntries(std::move(involvedEntries)), 
              m_violationCode(violationCode)
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CViolation(CRawEntry involvedEntry, ViolationCode violationCode)
            : m_involvedEntries(std::move(std::vector<CRawEntry> { involvedEntry })),
              m_violationCode(violationCode)
        {}

        
        /// @brief Constructor with @see {involvedEntries}, @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CViolation(
                   std::vector<CRawEntry>&& involvedEntries, 
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   ViolationCode violationCode) 
            : m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation),
              m_involvedEntries(std::move(involvedEntries)),
              m_violationCode(violationCode)
        {}

        /// @brief Constructor with only @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CViolation(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   ViolationCode violationCode) 
            : m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation),
              m_violationCode(violationCode)
        {}        

    public:
        /// @brief Returns back a list of raw entries involved in the current parsed component of 
        ///        the current linker script.
        const std::vector<CRawEntry>& InvoledEntries()
        {
            return this->m_involvedEntries;
        }

        /// @brief Reports back the raw entry just before the violation starts        
        const CRawEntry EntryBeforeViolation()
        {
            return this->m_entryBeforeViolation;
        }

        /// @brief Reports back the raw entry just after the violation ends        
        const CRawEntry EntryAfterViolation()
        {
            return this->m_entryAfterViolation;
        }

        /// @brief Reports back the tpe of violation        
        const ViolationCode Code()
        {
            return m_violationCode;
        }
    };
}

#endif
