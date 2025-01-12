#ifndef CASSIGNMENT_PROCEDURE_STATEMENT_H__
#define CASSIGNMENT_PROCEDURE_STATEMENT_H__

#include <utility>
#include <vector>
#include "CParsedContentBase.h"
#include "CAssignmentStatement.h"
#include "Raw/CRawEntry.h"


namespace VisualLinkerScript::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentProcedureStatement : public CParsedContentBase
    { 
    private:
        CRawEntry m_procedureNameEntry;
        CRawEntry m_parenthesisOpenEntry;
        CRawEntry m_parenthesisCloseEntry;
        CRawEntry m_delimiterOperator;
        std::shared_ptr<CAssignmentStatement> m_assignmentStatement;
        LinqVector<CParsedContentBase> m_parsedContent;

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentProcedureStatement(const CRawEntry& procedureNameEntry,
                                               const CRawEntry& parenthesisOpenEntry,
                                               const CRawEntry& parenthesisCloseEntry,
                                               const std::shared_ptr<CAssignmentStatement>& assignmentStatement,
                                               const CRawEntry& semicolonOperator,
                                               const LinqVector<CParsedContentBase>& parsedContent,
                                               const std::vector<CRawEntry>& rawElements,
                                               const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements,violations),
              m_procedureNameEntry(procedureNameEntry),
              m_parenthesisOpenEntry(parenthesisOpenEntry),
              m_parenthesisCloseEntry(parenthesisCloseEntry),
              m_delimiterOperator(semicolonOperator),
              m_assignmentStatement(assignmentStatement),
              m_parsedContent(parsedContent)
        {}

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::AssignmentStatement;
        }    

        /// @brief Gets the "ProcedureName"
        [[nodiscard]] CRawEntry ProcedureNameEntry() const
        {
            return this->m_procedureNameEntry;
        }       

        /// @brief Gets the "ParenthesisCloseEntry"
        [[nodiscard]] CRawEntry ParenthesisCloseEntry() const
        {
            return this->m_parenthesisCloseEntry;
        }   

        /// @brief Gets the "ParenthesisOpenEntry"
        [[nodiscard]] CRawEntry ParenthesisOpenEntry() const
        {
            return this->m_parenthesisOpenEntry;
        } 

        /// @brief Gets the "AssignmentStatement"
        [[nodiscard]] std::shared_ptr<CAssignmentStatement> AssignmentStatement() const
        {
            return this->m_assignmentStatement;
        }        

        /// @brief Gets the "SemicolonOperator"
        [[nodiscard]] const CRawEntry& DelimiterOperator() const
        {
            return this->m_delimiterOperator;
        }

        /// @brief Gets the "Parsed Content". This scenario it is most likely to contain comments only...
        [[nodiscard]] const LinqVector<CParsedContentBase>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
