#ifndef CPROCEDURE_CALL_STATEMENT_H__
#define CPROCEDURE_CALL_STATEMENT_H__

#include <vector>
#include "Models/CParsedContentBase.h"
#include "Models/CAssignmentStatement.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a procedural call
    class CProcedureCall : public CParsedContentBase
    {   
    private:
        CRawEntry m_procedureName;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        CRawEntry m_terminatingSemicolon;
        CAssignmentStatement m_assignmentStatement;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CProcedureCall(const CRawEntry& procedureName,
                                const CRawEntry& openingParenthesis,
                                const CRawEntry& closingParenthesis,
                                const CRawEntry& terminatingSemicolon,    
                                const CAssignmentStatement& assignmentStatement,
                                const std::vector<CRawEntry>& rawElements, 
                                const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_procedureName(procedureName),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_terminatingSemicolon(terminatingSemicolon),
              m_assignmentStatement(assignmentStatement)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProcedureCall;
        }

        /// @brief Reports back the assignment-statement
        [[nodiscard]] const CAssignmentStatement& AssignmentStatement() const
        {
            return this->m_assignmentStatement;
        }

        /// @brief Reports back the procedure-name
        [[nodiscard]] const CRawEntry& ProcedureName() const
        {
            return this->m_procedureName;
        }

        /// @brief Reports back the openning parenthesis
        [[nodiscard]] const CRawEntry& OpeningParenthesis() const
        {
            return this->m_openingParenthesis;
        }

        /// @brief Reports back the closing parenthesis
        [[nodiscard]] const CRawEntry& ClosingParenthesis() const
        {
            return this->m_closingParenthesis;
        }

        /// @brief Reports back the terminating semicolon at the end of the procedure call.
        [[nodiscard]] const CRawEntry& TerminatingSemicolon() const
        {
            return this->m_terminatingSemicolon;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] virtual const LinqVector<CViolationBase> AggregateViolation() const;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
