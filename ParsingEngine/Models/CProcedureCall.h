#ifndef CPROCEDURE_CALL_STATEMENT_H__
#define CPROCEDURE_CALL_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CAssignmentStatement.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a procedural call
    class CProcedureCall : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_procedureName;
        CRawEntry m_openningParenthesis;
        CRawEntry m_closingParenthesis;
        CRawEntry m_terminatingSemicolon;
        CAssignmentStatement m_assignmentStatement;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CProcedureCall(CRawEntry procedureName,
                                CRawEntry openningParenthesis,
                                CRawEntry closingParenthesis,
                                CRawEntry terminatingSemicolon,    
                                CAssignmentStatement&& assignmentStatement,
                                std::vector<CRawEntry>&& rawElements, 
                                std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(rawElements, violations),
              m_procedureName(procedureName),
              m_openningParenthesis(openningParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_terminatingSemicolon(terminatingSemicolon),
              m_assignmentStatement(std::move(assignmentStatement))
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProcedureCall;
        }

        /// @brief Reports back the assignment-statement
        const CAssignmentStatement& AssignmentStatmeent()
        {
            return this->m_assignmentStatement;
        }

        /// @brief Reports back the procedure-name
        const CRawEntry& ProcedureName()
        {
            return this->m_procedureName;
        }

        /// @brief Reports back the openning parenthesis
        const CRawEntry& OpenningParenthesis()
        {
            return this->m_openningParenthesis;
        }

        /// @brief Reports back the closing parenthesis
        const CRawEntry& ClosingParenthesis()
        {
            return this->m_closingParenthesis;
        }

        /// @brief Reports back the terminating semicolon at the end of the procedure call.
        const CRawEntry& TerminatingSemicolon()
        {
            return this->m_terminatingSemicolon;
        }       
    };
}

#endif
