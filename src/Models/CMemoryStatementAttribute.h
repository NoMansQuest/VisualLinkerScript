#ifndef CMEMORY_STATEMENT_ATTRIBUTE_H__
#define CMEMORY_STATEMENT_ATTRIBUTE_H__

#include <vector>
#include <memory>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents the type of attribute set for a given @see {CMemoryStatementAttribute}
    enum class AttributeDefinitionState
    {
        Defined,
        DefinedAndNegated,
        Undefined
    };

    /// @brief Represents attributes of "MEMORY" entries found in the "MEMORIES" region
    class CMemoryStatementAttribute : public CParsedContentBase
    {
    private:
        CRawEntry m_parenthesisOpen;
        CRawEntry m_parenthesisClose;
        AttributeDefinitionState m_readOnlySection;
        AttributeDefinitionState m_readWriteSection;
        AttributeDefinitionState m_executableSection;
        AttributeDefinitionState m_allocatableSection;
        AttributeDefinitionState m_initializedSection;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatementAttribute(const CRawEntry& parenthesisOpen,
                                           const CRawEntry& parenthesisClose,
                                           const AttributeDefinitionState& readOnlySection,
                                           const AttributeDefinitionState& readWriteSection,
                                           const AttributeDefinitionState& executableSection,
                                           const AttributeDefinitionState& allocatableSection,
                                           const AttributeDefinitionState& initializedSection,
                                           const std::vector<CRawEntry>& rawEntries,
                                           const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawEntries, violations),
              m_parenthesisOpen(parenthesisOpen),
              m_parenthesisClose(parenthesisClose),
              m_readOnlySection(readOnlySection),
              m_readWriteSection(readWriteSection),
              m_executableSection(executableSection),
              m_allocatableSection(allocatableSection),
              m_initializedSection(initializedSection)
        {}

        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatementAttribute()
            : CParsedContentBase({}, {}),
              m_readOnlySection(AttributeDefinitionState::Undefined),
              m_readWriteSection(AttributeDefinitionState::Undefined),
              m_executableSection(AttributeDefinitionState::Undefined),
              m_allocatableSection(AttributeDefinitionState::Undefined),
              m_initializedSection(AttributeDefinitionState::Undefined)
        {}

        /// @brief Assignment operator
        CMemoryStatementAttribute& operator= (CMemoryStatementAttribute& cloneSource)
        {
            this->m_parenthesisOpen = cloneSource.ParenthesisOpen();
            this->m_parenthesisClose = cloneSource.ParenthesisClose();
            this->m_allocatableSection = cloneSource.AllocatableSection();
            this->m_executableSection = cloneSource.ExecutableSection();
            this->m_readOnlySection = cloneSource.ReadOnlySection();
            this->m_readWriteSection = cloneSource.ReadWriteSection();
            this->m_initializedSection = cloneSource.InitializedSection();
            return *this;
        }

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::MemoryStatementAttribute;
        }

        /// @brief Reports back 'Parenthesis-Open' entry
        [[nodiscard]] CRawEntry ParenthesisOpen()
        {
            return m_parenthesisOpen;
        }

        /// @brief Reports back 'Parenthesis-Close' entry
        [[nodiscard]] CRawEntry ParenthesisClose()
        {
            return m_parenthesisClose;
        }

        /// @brief Reports back the Attributes of the statement.
        [[nodiscard]] AttributeDefinitionState ReadOnlySection() const
        {
            return this->m_readOnlySection;
        }

        /// @brief Reports back the state of 'ReadWriteSection' attribute
        [[nodiscard]] AttributeDefinitionState ReadWriteSection() const
        {
            return this->m_readWriteSection;
        }

        /// @brief Reports back the state of 'ExecutableSection' attribute
        [[nodiscard]] AttributeDefinitionState ExecutableSection() const
        {
            return this->m_executableSection;
        }

        /// @brief Reports back the state of 'AllocatableSection' attribute
        [[nodiscard]] AttributeDefinitionState AllocatableSection() const
        {
            return this->m_allocatableSection;
        }

        /// @brief Reports back the state of 'InitializedSection' attribute
        [[nodiscard]] AttributeDefinitionState InitializedSection() const
        {
            return this->m_initializedSection;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();
        }
    };
}

#endif
