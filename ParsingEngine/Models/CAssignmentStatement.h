#ifndef CASSIGNMENT_STATEMENT_H__
#define CASSIGNMENT_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CExpression.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentStatement : public CLinkerScriptContentBase
    { 
    private:
        std::vector<CRawEntry> m_assignmentOperator;        
        CExpression m_lValueExpression;
        CExpression m_rValueExpression;
        CRawEntry m_deliminterOperator;

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentStatement(CExpression&& lValueExpression,
                                      CExpression&& rValueExpression,
                                      std::vector<CRawEntry>&& assignmentOperator,
                                      CRawEntry semicolonOperator,
                                      std::vector<CRawEntry>&& rawElements, 
                                      std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_assignmentOperator(std::move(assignmentOperator)),
              m_lValueExpression(std::move(lValueExpression)),
              m_rValueExpression(std::move(rValueExpression)),
              m_deliminterOperator(semicolonOperator)
        {}

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::AssignmentStatement;
        }    

        /// @brief Gets the "AssignmentOperator"
        const std::vector<CRawEntry>& AssignmentOperator()
        {
            return this->m_assignmentOperator;
        }        

        /// @brief Gets the "LValueComposition"
        const CExpression& LValueExpression()
        {
            return this->m_lValueExpression;
        }
        
        /// @brief Gets the "RValueComposition"
        const CExpression& RValueExpression()
        {
            return this->m_rValueExpression;
        }

        /// @brief Gets the "SemicolonOperator"
        const CRawEntry& DelimiterOperator()
        {
            return this->m_deliminterOperator;
        }
    };
}

#endif
