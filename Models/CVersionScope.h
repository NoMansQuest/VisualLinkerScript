#ifndef CVERSION_SCOPE_H__
#define CVERSION_SCOPE_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in a version-node inside the "VERSIONS" region
    class CVersionScope : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_scopeNameEntry;
        CRawEntry m_inheritingScopeEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        CRawEntry m_semicolonDelimiterEntry;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param versionName Name of the version we'de defining
        /// @param versionDependsOn (Optional) Name of the version this entry extends upon.
        /// @param openingBracket Entry containing the opening curly-brakcet
        /// @param closingBracket Entry containing the closing brakcet
        /// @param semicolonDelimiter Entry containing the semicolon delimiter
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CVersionScope(CRawEntry scopeNameEntry,
                               CRawEntry inheritingScopeEntry,
                               CRawEntry openingBracketEntry,
                               CRawEntry closingBracketEntry,
                               CRawEntry semicolonDelimiterEntry,
                               std::vector<std::shared_ptr<CLinkerScriptContentBase>> parsedContent,
                               std::vector<CRawEntry>&& rawElements,
                               std::vector<CParserViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
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
        CRawEntry ScopeNameEntry()
        {
            return this->m_scopeNameEntry;
        }

        /// @brief Reports back inheriting scope entry
        CRawEntry InheritingScopeEntry()
        {
            return this->m_inheritingScopeEntry;
        }

        /// @brief Reports back opening bracket entry
        CRawEntry OpeningBracketEntry()
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back closing bracket entry
        CRawEntry ClosingBracketEntry()
        {
            return this->m_closingBracketEntry;
        }

        /// @brief Reports back parsed content
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>> ParsedContent()
        {
            return this->m_parsedContent;
        }

    };
}

#endif
