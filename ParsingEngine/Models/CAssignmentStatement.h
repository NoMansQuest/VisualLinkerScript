#ifndef CASSIGNMENT_STATEMENT_H__
#define CASSIGNMENT_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentStatement : public CLinkerScriptContentBase
    { 
    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::AssignmentStatement;
        }    

    private:
        std::vector<CRawEntry> m_assignmentOperator;        
        std::vector<CLinkerScriptContentBase> m_lValueComposition;
        std::vector<CLinkerScriptContentBase> m_rValueComposition;
        CRawEntry m_semicolonOperator;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CAssignmentStatement(std::vector<CLinkerScriptContentBase>&& lValueComposition,
                                      std::vector<CLinkerScriptContentBase>&& rValueComposition,
                                      std::vector<CRawEntry>&& assignmentOperator,
                                      std::CRawEntry semicolonOperator,
                                      std::vector<CRawEntry>&& rawElements, 
                                      std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_lValueComposition(lValueComposition),
              m_rValueComposition(rValueComposition),
              m_assignmentOperator(assignmentOperator),
              m_semicolonOperator(semicolonOperator)
        {}        

    public:
        /// @brief Gets the "AssignmentOperator"
        std::vector<CRawEntry>& AssignmentOperator const
        {
            return this->m_assignmentOperator;
        }        

        /// @brief Gets the "LValueComposition"
        std::vector<CLinkerScriptContentBase>& LValueComposition const
        {
            return this->m_lValueComposition;
        }
        
        /// @brief Gets the "RValueComposition"
        std::vector<CLinkerScriptContentBase>& RValueComposition const
        {
            return this->m_rValueComposition;
        }

        /// @brief Gets the "SemicolonOperator"
        CRawEntry& SemicolonOperator() const
        {
            return this->m_semicolonOperator;
        }
    }
}

#endif
