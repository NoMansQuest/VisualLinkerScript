#ifndef CASSIGNMENT_PROCEDURE_STATEMENT_H__
#define CASSIGNMENT_PROCEDURE_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CAssignmentStatement.h"
#include "Raw/CRawEntry.h"


namespace VisualLinkerScript::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentProcedureStatement : public CLinkerScriptContentBase
    { 
    private:
        CRawEntry m_procedureNameEntry;
        CRawEntry m_parenthesisOpenEntry;
        CRawEntry m_parenthesisCloseEntry;
        std::shared_ptr<CAssignmentStatement> m_assignmentStatement;
        CRawEntry m_deliminterOperator;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentProcedureStatement(CRawEntry procedureNameEntry,
                                               CRawEntry parenthesisOpenEntry,
                                               CRawEntry parenthesisCloseEntry,
                                               std::shared_ptr<CAssignmentStatement> assignmentStatement,
                                               CRawEntry semicolonOperator,
                                               std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                               std::vector<CRawEntry>&& rawElements,
                                               SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_procedureNameEntry(procedureNameEntry),
              m_parenthesisOpenEntry(parenthesisOpenEntry),
              m_parenthesisCloseEntry(parenthesisCloseEntry),
              m_assignmentStatement(assignmentStatement),
              m_deliminterOperator(semicolonOperator),
              m_parsedContent(std::move(parsedContent))
        {}

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::AssignmentStatement;
        }    

        /// @brief Gets the "ProcedureName"
        const CRawEntry ProcedureNameEntry()
        {
            return this->m_procedureNameEntry;
        }       

        /// @brief Gets the "ParenthesisCloseEntry"
        const CRawEntry ParenthesisCloseEntry()
        {
            return this->m_parenthesisCloseEntry;
        }   

        /// @brief Gets the "ParenthesisOpenEntry"
        const CRawEntry ParenthesisOpenEntry()
        {
            return this->m_parenthesisOpenEntry;
        } 

        /// @brief Gets the "AssignmentStatement"
        std::shared_ptr<CAssignmentStatement> AssignmentStatement()
        {
            return this->m_assignmentStatement;
        }        

        /// @brief Gets the "SemicolonOperator"
        const CRawEntry& DelimiterOperator()
        {
            return this->m_deliminterOperator;
        }

        /// @brief Gets the "Parsed Content". This this scenario it is most likely to contain comments only...
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }
    };
}

#endif
