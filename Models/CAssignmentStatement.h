#ifndef CASSIGNMENT_STATEMENT_H__
#define CASSIGNMENT_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CExpression.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentStatement : public CLinkerScriptContentBase
    { 
    private:
        CRawEntry m_assignmentOperator;
        CRawEntry m_lValueEntry;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;
        CRawEntry m_semicolonEntry;

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentStatement(CRawEntry lValueEntry,
                                      CRawEntry assignmentOperator,
                                      CRawEntry semicolonEntry,
                                      std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                      std::vector<CRawEntry>&& rawElements, 
                                      SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_assignmentOperator(std::move(assignmentOperator)),
              m_lValueEntry(std::move(lValueEntry)),
              m_parsedContent(std::move(parsedContent)),
              m_semicolonEntry(semicolonEntry)
        {}

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::AssignmentStatement;
        }    

        /// @brief Gets the "AssignmentOperator"
        const CRawEntry AssignmentOperator() const
        {
            return this->m_assignmentOperator;
        }        

        /// @brief Gets the "LValue" entry
        const CRawEntry LValueEntry() const
        {
            return this->m_lValueEntry;
        }
        
        /// @brief Gets the "RValueComposition"
        const SharedPtrVector<CLinkerScriptContentBase>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Gets the "SemicolonEntry"
        const CRawEntry& SemicolonEntry() const
        {
            return this->m_semicolonEntry;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
