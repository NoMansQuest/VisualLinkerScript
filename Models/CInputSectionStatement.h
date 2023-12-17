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
        std::shared_ptr<CLinkerScriptContentBase> m_fileSelector;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;        

    public:
        /// @brief Default constructor, including desired sections
        explicit CInputSectionStatement(std::shared_ptr<CLinkerScriptContentBase> fileSelector,
                                        CRawEntry openingParenthesis,
                                        CRawEntry closingParenthesis,
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                        std::vector<CRawEntry>&& rawElements,
                                        SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_fileSelector(fileSelector),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(std::move(parsedContent))
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSection;
        }

        /// @brief Reports back the list of parameters
        const SharedPtrVector<CLinkerScriptContentBase>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the filter header
        std::shared_ptr<CLinkerScriptContentBase> FileSelector() const
        {
            return this->m_fileSelector;
        }

        /// @brief Reports back the opening parenthesis
        const CRawEntry& OpeningParenthesis() const
        {
            return this->m_openingParenthesis;
        }

        /// @brief Reports back the closing parenthesis
        const CRawEntry& ClosingParenthesis() const
        {
            return this->m_closingParenthesis;
        }

        /// @brief If true, the input-sections requested particular regions of target group
        const bool AreCodeSectionsSpecified() const
        {
            return m_openingParenthesis.IsPresent() &&
                   m_closingParenthesis.IsPresent() &&
                   !m_parsedContent.empty();
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
