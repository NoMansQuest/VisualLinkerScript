#ifndef CASSIGNMENT_PROCEDURE_STATEMENT_H__
#define CASSIGNMENT_PROCEDURE_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CAssignmentStatement.h"
#include "../Models/Raw/CRawEntry.h"


namespace VisualLinkerScript::ParsingEngine::Models
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

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentProcedureStatement(CRawEntry procedureNameEntry,
                                               CRawEntry parenthesisOpenEntry,
                                               CRawEntry parenthesisCloseEntry,
                                               std::shared_ptr<CAssignmentStatement> assignmentStatement,
                                               CRawEntry semicolonOperator,
                                               std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                               std::vector<CRawEntry>&& rawElements,
                                               std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_procedureNameEntry(procedureNameEntry),
              m_parenthesisOpenEntry(parenthesisOpenEntry),
              m_parenthesisCloseEntry(parenthesisCloseEntry),
              m_assignmentStatement(assignmentStatement),
              m_deliminterOperator(semicolonOperator)
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
    };
}

#endif
