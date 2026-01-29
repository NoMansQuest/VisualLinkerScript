#ifndef CVERSION_NODE_H__
#define CVERSION_NODE_H__

#include <vector>
#include "Models/CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CVersionNode : public CParsedContentBase
    {   
    private:
        CRawEntry m_nodeEntry;
        CRawEntry m_semicolonEntry;
        CRawEntry m_parentTagEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CVersionNode(const CRawEntry& nodeEntry,
                              const CRawEntry& semicolonEntry,
                              const CRawEntry& parentTagEntry,
                              const std::vector<CRawEntry>& rawElements,
                              const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_nodeEntry(nodeEntry),
              m_semicolonEntry(semicolonEntry),
              m_parentTagEntry(parentTagEntry)
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::VersionNode;
        }

        /// @brief Reports back the entry containing the 'Node' itself
        CRawEntry NodeEntry()
        {
            return this->m_nodeEntry;
        }

        /// @brief Reports back the entry containing the Semicolon.
        CRawEntry SemicolonEntry()
        {
            return this->m_semicolonEntry;
        }

        /// @brief Reports back the parent tage entry for this node.
        CRawEntry ParentTagEntry()
        {
            return this->m_parentTagEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();
        }
    };    
}

#endif
