#ifndef CASSIGNMENT_STATEMENT_H__
#define CASSIGNMENT_STATEMENT_H__

#include <vector>
#include "CParsedContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents assignment statements. They are not bound to a particular region and could almost be 
    ///        anywhere and everywhere. 
    class CAssignmentStatement : public CParsedContentBase
    { 
    private:
        CRawEntry m_assignmentOperator;
        CRawEntry m_lValueEntry;
        SharedPtrVector<CParsedContentBase> m_parsedContent;
        CRawEntry m_semicolonEntry;

    public:
        /// @brief Detailed Constructor
        explicit CAssignmentStatement(const CRawEntry& lValueEntry,
                                      const CRawEntry& assignmentOperator,
                                      const CRawEntry& semicolonEntry,
                                      const SharedPtrVector<CParsedContentBase>& parsedContent,
                                      const std::vector<CRawEntry>& rawElements, 
                                      const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_assignmentOperator(assignmentOperator),
              m_lValueEntry(lValueEntry),
              m_parsedContent(parsedContent),
              m_semicolonEntry(semicolonEntry)
        {}

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::AssignmentStatement;
        }    

        /// @brief Gets the "AssignmentOperator"
        [[nodiscard]] CRawEntry AssignmentOperator() const
        {
            return this->m_assignmentOperator;
        }        

        /// @brief Gets the "LValue" entry
        [[nodiscard]] CRawEntry LValueEntry() const
        {
            return this->m_lValueEntry;
        }
        
        /// @brief Gets the "RValueComposition"
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Gets the "SemicolonEntry"
        [[nodiscard]] const CRawEntry& SemicolonEntry() const
        {
            return this->m_semicolonEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
