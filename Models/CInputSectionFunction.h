#ifndef CINPUT_SECTION_FUNCTION_H__
#define CINPUT_SECTION_FUNCTION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an input-section function call where paramrers aren't separated with comma.
    /// @brief Example: the 'EXCLUDE_FILE' call in the statement '*(EXCLUDE_FILE (*crtend.o *otherfile.o) .ctors)'
    class CInputSectionFunction : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_functionName;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CInputSectionFunction(CRawEntry functionName,
                                       CRawEntry openingParenthesis,
                                       CRawEntry closingParenthesis,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                       std::vector<CRawEntry>&& rawElements,
                                       SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_functionName(functionName),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(std::move(parsedContent))
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::InputSectionFunction;
        }

        /// @brief Reports back the list of parameters
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the function name
        const CRawEntry& FunctionName() const
        {
            return this->m_functionName;
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

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
