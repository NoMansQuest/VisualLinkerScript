#ifndef CINPUT_SECTION_FUNCTION_H__
#define CINPUT_SECTION_FUNCTION_H__

#include <vector>
#include "Models/CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an input-section function call where paramrers aren't separated with comma.
    /// @brief Example: the 'EXCLUDE_FILE' call in the statement '*(EXCLUDE_FILE (*crtend.o *otherfile.o) .ctors)'
    class CInputSectionFunction : public CParsedContentBase
    {
    private:
        CRawEntry m_functionName;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        std::vector<std::shared_ptr<CParsedContentBase>> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CInputSectionFunction(const CRawEntry& functionName,
                                       const CRawEntry& openingParenthesis,
                                       const CRawEntry& closingParenthesis,
                                       const LinqVector<CParsedContentBase>& parsedContent,
                                       const std::vector<CRawEntry>& rawElements,
                                       const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_functionName(functionName),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(parsedContent)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionFunction;
        }

        /// @brief Reports back the list of parameters
        [[nodiscard]] const std::vector<std::shared_ptr<CParsedContentBase>>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the function name
        [[nodiscard]] const CRawEntry& FunctionName() const
        {
            return this->m_functionName;
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

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
