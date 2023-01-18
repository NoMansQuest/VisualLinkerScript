#ifndef CMEMORY_STATEMENT_ATTRIBUTE_H__
#define CMEMORY_STATEMENT_ATTRIBUTE_H__

#include <vector>
#include <memory>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the type of attribute set for a given @see {CMemoryStatementAttribute}
    enum class AttributeDefinitionState
    {
        Defined,
        DefinedAndNegated,
        Undefined
    };

    /// @brief Represents attributes of "MEMORY" entries found in the "MEMORIES" region
    class CMemoryStatementAttribute : public CLinkerScriptContentBase
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
        explicit CMemoryStatementAttribute(CRawEntry parenthesisOpen,
                                           CRawEntry parenthesisClose,
                                           AttributeDefinitionState readOnlySection,
                                           AttributeDefinitionState readWriteSection,
                                           AttributeDefinitionState executableSection,
                                           AttributeDefinitionState allocatableSection,
                                           AttributeDefinitionState initializedSection,
                                           std::vector<CRawEntry>&& rawEntries,
                                           std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawEntries), std::move(violations)),
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
            : CLinkerScriptContentBase({}, {}),
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
        const CRawEntry ParenthesisOpen()
        {
            return m_parenthesisOpen;
        }

        /// @brief Reports back 'Parenthesis-Close' entry
        const CRawEntry ParenthesisClose()
        {
            return m_parenthesisClose;
        }

        /// @brief Rpeorts back the Attributes of the statement.
        const AttributeDefinitionState ReadOnlySection()
        {
            return this->m_readOnlySection;
        }

        /// @brief Reports back the state of 'ReadWriteSection' attribute
        const AttributeDefinitionState ReadWriteSection()
        {
            return this->m_readWriteSection;
        }

        /// @brief Reports back the state of 'ExecutableSection' attribute
        const AttributeDefinitionState ExecutableSection()
        {
            return this->m_executableSection;
        }

        /// @brief Reports back the state of 'AllocatableSection' attribute
        const AttributeDefinitionState AllocatableSection()
        {
            return this->m_allocatableSection;
        }

        /// @brief Reports back the state of 'InitializedSection' attribute
        const AttributeDefinitionState InitializedSection()
        {
            return this->m_initializedSection;
        }
    };
}

#endif
