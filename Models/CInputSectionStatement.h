#ifndef CINPUT_SECTION_H__
#define CINPUT_SECTION_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Input-Section declaration
    /// @brief Example: *(.data)
    class CInputSectionStatement : public CParsedContentBase
    {
    private:
        std::shared_ptr<CParsedContentBase> m_fileSelector;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        SharedPtrVector<CParsedContentBase> m_parsedContent;        

    public:
        /// @brief Default constructor, including desired sections
        explicit CInputSectionStatement(const std::shared_ptr<CParsedContentBase>& fileSelector,
                                        const CRawEntry& openingParenthesis,
                                        const CRawEntry& closingParenthesis,
                                        const SharedPtrVector<CParsedContentBase>& parsedContent,
                                        const std::vector<CRawEntry>& rawElements,
                                        const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_fileSelector(fileSelector),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(parsedContent)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSection;
        }

        /// @brief Reports back the list of parameters
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the filter header
        [[nodiscard]] std::shared_ptr<CParsedContentBase> FileSelector() const
        {
            return this->m_fileSelector;
        }

        /// @brief Reports back the opening parenthesis
        [[nodiscard]] const CRawEntry& OpeningParenthesis() const
        {
            return this->m_openingParenthesis;
        }

        /// @brief Reports back the closing parenthesis
        [[nodiscard]] const CRawEntry& ClosingParenthesis() const
        {
            return this->m_closingParenthesis;
        }

        /// @brief If true, the input-sections requested particular regions of target group
        [[nodiscard]] bool AreCodeSectionsSpecified() const
        {
            return m_openingParenthesis.IsPresent() &&
                   m_closingParenthesis.IsPresent() &&
                   !m_parsedContent.empty();
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
