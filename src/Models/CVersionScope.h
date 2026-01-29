#ifndef CVERSION_SCOPE_H__
#define CVERSION_SCOPE_H__

#include <vector>
#include "Models/CParsedContentBase.h"
#include "Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in a version-node inside the "VERSIONS" region
    class CVersionScope : public CParsedContentBase
    {   
    private:
        CRawEntry m_scopeNameEntry;
        CRawEntry m_inheritingScopeEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_semicolonDelimiterEntry;
        LinqVector<CParsedContentBase> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CVersionScope(const CRawEntry& scopeNameEntry,
                               const CRawEntry& inheritingScopeEntry,
                               const CRawEntry& openingBracketEntry,
                               const CRawEntry& closingBracketEntry,
                               const CRawEntry& semicolonDelimiterEntry,
                               const LinqVector<CParsedContentBase>& parsedContent,
                               const std::vector<CRawEntry>& rawElements,
                               const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_scopeNameEntry(scopeNameEntry),
              m_inheritingScopeEntry(inheritingScopeEntry),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_semicolonDelimiterEntry(semicolonDelimiterEntry),
              m_parsedContent(parsedContent)
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::VersionScope;
        }

        /// @brief Reports back the name of the scope
        [[nodiscard]] CRawEntry ScopeNameEntry() const
        {
            return this->m_scopeNameEntry;
        }

        /// @brief Reports back inheriting scope entry
        [[nodiscard]] CRawEntry InheritingScopeEntry() const
        {
            return this->m_inheritingScopeEntry;
        }

        /// @brief Reports back opening bracket entry
        [[nodiscard]] CRawEntry OpeningBracketEntry() const
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back closing bracket entry
        [[nodiscard]] CRawEntry ClosingBracketEntry() const
        {
            return this->m_closingBracketEntry;
        }

        /// @brief Reports back parsed content
        [[nodiscard]] std::vector<std::shared_ptr<CParsedContentBase>> ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] virtual const LinqVector<CViolationBase> AggregateViolation() const;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
