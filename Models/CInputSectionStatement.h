#ifndef CINPUT_SECTION_H__
#define CINPUT_SECTION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Input-Section declaration
    /// @brief Example: *(.data)
    class CInputSectionStatement : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_filterHeader;        
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;        

    public:
        /// @brief Default constructor, including desired sections
        explicit CInputSectionStatement(CRawEntry filterHeader,
                                        CRawEntry openingParenthesis,
                                        CRawEntry closingParenthesis,
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                        std::vector<CRawEntry>&& rawElements,
                                        std::vector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_filterHeader(filterHeader),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(std::move(parsedContent))
        {}

        /// @brief Default constructor, including desired sections
        explicit CInputSectionStatement(CRawEntry filterHeader,
                                        std::vector<CRawEntry>&& rawElements,
                                        std::vector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_filterHeader(filterHeader)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSection;
        }

        /// @brief Reports back the list of parameters
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the filter header
        const CRawEntry& FilterHeader()
        {
            return this->m_filterHeader;
        }

        /// @brief Reports back the opening parenthesis
        const CRawEntry& OpeningParenthesis()
        {
            return this->m_openingParenthesis;
        }

        /// @brief Reports back the closing parenthesis
        const CRawEntry& ClosingParenthesis()
        {
            return this->m_closingParenthesis;
        }

        /// @brief If true, the input-sections requested particular regions of target group
        const bool AreCodeSectionsSpecified()
        {
            return m_openingParenthesis.IsPresent() &&
                   m_closingParenthesis.IsPresent() &&
                   !m_parsedContent.empty();
        }
    };
}

#endif
