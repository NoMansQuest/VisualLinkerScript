#ifndef CLINKERSCRIPT_CONTENT_BASE_H__
#define CLINKERSCRIPT_CONTENT_BASE_H__

#include <vector>
#include <memory>
#include <string>
#include "Raw/CRawEntry.h"
#include "../Helpers.h"

namespace VisualLinkerScript::Models
{
    class CViolationBase;
    class CLinkerScriptFile;
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief Type of content. This is to simplifying type-casting
    enum class ContentType
    {
        UnrecognizableContent,
        Comment,
        ProcedureCall,
        FunctionCall,
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
        SectionOutputConstraint,
        SectionOutputAtLmaRegion,
        SectionOutputToRegion,
        SectionsOverlayStatement,
        SectionOutputFillExpression,
        SectionOutputPhdr,              // Example: :phdr
        SectionOutputDataExpression,    // Mainly 'CREATE_OBJECT_SYMBOLS'
        InputSectionTargetSection,      // Example: *data.*(... <InputSectionTargetSection> ... )
        InputSectionWildcardWord,       // Example: *data?.ctor.[A-Za-z0-9]*
        InputSectionFunction,           // Example: SORT(*), EXCLUDE_FILE(....)
        InputSection,                   // Example: *data.*(... target-Sections ... )
        VersionsRegion,        
        VersionNode,
        VersionTag,
        VersionScope,
        VersionScopeEntry,
        WildcardEntry,                  // For instance: *.text.???.o
        PrimarySymbol,
        Number,
        SecondarySymbol
    };

    /// @brief Linker-Script content based object.
    class CLinkerScriptContentBase
    {
    private:
        std::vector<CRawEntry> m_rawEntries;
        SharedPtrVector<CViolationBase> m_violations;
        std::shared_ptr<CLinkerScriptFile> m_parentLinkerScriptFile;
        std::string m_objectPath;

    protected:
        /// @brief Constructor, accessible to inheritors only
        /// @param rawEntries A list of object this element is comprised of
        /// @param violations Violations related to this content (if any)
        explicit CLinkerScriptContentBase(const std::vector<CRawEntry>& rawEntries,
                                          const SharedPtrVector<CViolationBase>& violations)
            : m_rawEntries(rawEntries),
              m_violations(violations)
        {
	        // No action done ehere.
        }

    public:
        /// @brief Reports back the type of this object       
        virtual ContentType Type() = 0;

        /// @brief Does content pass integrity check?
        bool IsValid() const
        {
            return m_violations.empty();
        }

        /// @brief Returns the first element present        
        const std::vector<CRawEntry>& RawEntries() const
        {            
            return m_rawEntries;
        }

        /// @brief Returns list detected violations
        const SharedPtrVector<CViolationBase>& Violations() const
        {
            return m_violations;
        }

        /// @brief Content-Sensitive path.
        void SetObjectPath(std::string objectPath){
            this->m_objectPath = objectPath;
        }

        /// @brief Returns object's path. Example:
        /// @remarks '<linker-script-file-name>/<region>/[sub-region]/<object-type>#index
        const std::string ObjectPath() const {
            return this->m_objectPath;
        }

        /// @brief Returns list detected violations
        uint32_t StartPosition() const
        {
            return this->m_rawEntries[0].StartPosition();
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const;
    };
}

#endif
