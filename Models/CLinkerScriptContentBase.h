#ifndef CLINKERSCRIPT_CONTENT_BASE_H__
#define CLINKERSCRIPT_CONTENT_BASE_H__

#include <vector>
#include "CParserViolation.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Type of content. This is to simplifying type-casting
    enum class ContentType
    {
        UnrecognizableContent,
        Comment,
        ProcedureCall,
        AssignmentStatement,
        Expression,
        ArithmeticOrLogicalOperator,
        EnclosedExpression,        
        PhdrsRegion,
        PhdrsStatement,
        ParameterSeparator,
        MemoryRegion,
        MemoryStatement,
        MemoryStatementAttribute,
        MemoryStatementAttributeValue,
        SectionsRegion,
        SectionsOutputCommand,
        SectionOutputType,
        SectionOutputAtLmaRegion,
        SectionOutputToRegion,
        SectionsOverlayStatement,
        SectionOutputFillExpression,
        SectionOutputPhdr,              // Example: :phdr
        InputSectionTargetSection,      // Example: *data.*(... <InputSectionTargetSection> ... )
        InputSection,                   // Example: *data.*(... target-Sections ... )
        VersionsRegion,        
        VersionNode,
        VersionTag,
        VersionScope,
        VersionScopeEntry,
        PrimarySymbol,
        Number,
        SecondarySymbol
    };

    /// @brief Linker-Script content based object.
    class CLinkerScriptContentBase
    {
    private:
        std::vector<CRawEntry> m_rawEntries;
        std::vector<CParserViolation> m_violations;

    protected:
        /// @brief Constructor, accessible to inheritors only
        /// @param rawEntries A list of object this element is comprised of
        /// @param violations Violations related to this content (if any)
        explicit CLinkerScriptContentBase(std::vector<CRawEntry>&& rawEntries,
                                          std::vector<CParserViolation>&& violations)
            : m_rawEntries(std::move(rawEntries)),
              m_violations(std::move(violations))
        {}

    public:
        /// @brief Reports back the type of this object       
        virtual ContentType Type() = 0;

        /// @brief Does content pass integrity check?
        bool IsValid()
        {
            return m_violations.size() == 0;
        }

        /// @brief Returns the first element present        
        const std::vector<CRawEntry>& RawEntries()
        {
            return m_rawEntries;
        }

        /// @brief Returns list detected violations
        const std::vector<CParserViolation>& Violations()
        {
            return m_violations;
        }
    };
}

#endif
