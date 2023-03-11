#ifndef CASSIGNMENT_PROCEDURE_STATEMENT_H__
#define CASSIGNMENT_PROCEDURE_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CExpression.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentProcedureStatement : public CLinkerScriptContentBase
    { 
    private:
        CRawEntry m_procedureName;
        CRawEntry m_parenthesisOpenEntry;
        CRawEntry m_parenthesisCloseEntry;
        CRawEntry m_assignmentOperator;
        CRawEntry m_lValueEntry;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;
        CRawEntry m_deliminterOperator;

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentStatement(CRawEntry procedureName,
                                      CRawEntry parenthesisOpenEntry,
                                      CRawEntry parenthesisCloseEntry,  
                                      CRawEntry lValueEntry,
                                      CRawEntry assignmentOperator,
                                      CRawEntry semicolonOperator,
                                      std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                      std::vector<CRawEntry>&& rawElements, 
                                      std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_procedureName(procedureName),
              m_parenthesisOpenEntry(parenthesisOpenEntry),
              m_parenthesisCloseEntry(parenthesisClosEntry),
              m_assignmentOperator(std::move(assignmentOperator)),
              m_lValueEntry(std::move(lValueEntry)),
              m_parsedContent(std::move(parsedContent)),
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

        /// @brief Gets the "SemicolonOperator"
        const CRawEntry& DelimiterOperator()
        {
            return this->m_deliminterOperator;
        }
    };
}

#endif
