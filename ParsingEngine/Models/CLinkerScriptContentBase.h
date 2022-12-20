#ifndef CLINKERSCRIPT_CONTENT_BASE_H__
#define CLINKERSCRIPT_CONTENT_BASE_H__

#include <vector>
#include "CViolation.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Type of content. This is to simplifying type-casting
    enum class ContentType
    {
        UnrecognizableContent,
        Comment,
        ProcedureCall,
        AssignmentStatement,
        CCompoundExpression,
        PhdrsRegion,
        PhdrsStatement,
        MemoryRegion,
        MemoryStatement,
        SectionsRegion,
        SectionsOutputStatement,
        SectionsOverlayStatement,
        VersionsRegion,        
        VersionNode,
        VersionScope,
        VersionScopeEntry,
        PrimarySymbol,
        SecondarySymbol
    };

    /// @brief Linker-Script content based object.
    class CLinkerScriptContentBase
    {
    private:
        std::vector<CRawElement> m_rawElements;
        std::vector<CViolation> m_violations;

    protected:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of
        /// @param violations Violations related to this content (if any)
        explicit CLinkerScriptContentBase(std::vector<CRawEntry>&& rawElements, 
                                          std::vector<CViolation>&& violations)
            : m_rawElements(std::move(m_rawElements)), 
              m_violations(std::move(violations))
        {}

        /// @brief Default destructor. We don't hold any object in need of release.
        ~CLinkerScriptContentBase()
        {}

    public:
        /// @brief Reports back the type of this object       
        virtual ContentType Type() = 0;                

        /// @brief Does content pass integrity check? 
        bool IsValid()
        {
            return violations.size() == 0;
        }

        /// @brief Returns the first element present        
        const std::vector<CRawElement>& RawElements()
        {
            return m_rawElements;
        }

        /// @brief Returns list detected violations
        const std::vector<CViolation>& Violations()
        {
            return m_violations;
        }
    }
}

#endif