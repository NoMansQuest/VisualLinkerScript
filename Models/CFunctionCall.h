#ifndef CFUNCTION_CALL_STATEMENT_H__
#define CFUNCTION_CALL_STATEMENT_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    using namespace VisualLinkerScript;

    /// @brief Represents a function call
    /// @brief: Example: ALIGN(4)
    class CFunctionCall : public CParsedContentBase
    {   
    private:
        CRawEntry m_functionName;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        SharedPtrVector<CParsedContentBase> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CFunctionCall(const CRawEntry& functionName,
                               const CRawEntry& openingParenthesis,
                               const CRawEntry& closingParenthesis,
                               const SharedPtrVector<CParsedContentBase>& parsedContent,
                               const std::vector<CRawEntry>& rawElements,
                               const SharedPtrVector<CViolationBase>& violations)
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
            return ContentType::FunctionCall;
        }

        /// @brief Reports back the list of parameters
        [[nodiscard]] const std::vector<std::shared_ptr<CParsedContentBase>>& ParsedContent() const
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the procedure-name
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
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
