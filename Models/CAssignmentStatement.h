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
                                      std::vector<CViolation>&& violations)
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
        const CRawEntry AssignmentOperator()
        {
            return this->m_assignmentOperator;
        }        

        /// @brief Gets the "LValue" entry
        const CRawEntry LValueEntry()
        {
            return this->m_lValueEntry;
        }
        
        /// @brief Gets the "RValueComposition"
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }

        /// @brief Gets the "SemicolonEntry"
        const CRawEntry& SemicolonEntry()
        {
            return this->m_semicolonEntry;
        }
    };
}

#endif
