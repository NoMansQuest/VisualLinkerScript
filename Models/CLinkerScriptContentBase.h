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
        SharedPtrVector<CViolationBase> m_violations;
        std::shared_ptr<CLinkerScriptFile> m_parentLinkerScriptFile;
        std::string m_objectPath;

    protected:
        /// @brief Constructor, accessible to inheritors only
        /// @param rawEntries A list of object this element is comprised of
        /// @param violations Violations related to this content (if any)
        explicit CLinkerScriptContentBase(std::vector<CRawEntry>&& rawEntries,
                                          SharedPtrVector<CViolationBase>&& violations)
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
        const SharedPtrVector<CViolationBase>& Violations()
        {
            return m_violations;
        }

        /// @brief Content-Sensitive path.
        void SetObjectPath(std::string objectPath){
            this->m_objectPath = objectPath;
        }

        /// @brief Returns object's path. Example:
        /// @remarks '<linker-script-file-name>/<region>/[sub-region]/<object-type>#index
        std::string ObjectPath(){
            return this->m_objectPath;
        }

        /// @brief Returns list detected violations
        const uint32_t StartPosition()
        {
            return this->m_rawEntries[0].StartPosition();
        }

        /// @brief Updates the parent linker script file.
        /// @remarks This cannot be set via constructor, as the parent linker-script file is not
        ///          created until parsing is complete, which is long after this objects creation.
        void SetParentLinkerScriptfile(std::shared_ptr<CLinkerScriptFile> newParent) {
            this->m_parentLinkerScriptFile = newParent;
        }

        /// @brief Returns the ParentLinkerScriptFile for this object.
        std::shared_ptr<CLinkerScriptFile> ParentLinkerScriptFile(){
            return this->m_parentLinkerScriptFile;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth);
    };
}

#endif
