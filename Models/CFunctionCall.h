#ifndef CFUNCTION_CALL_STATEMENT_H__
#define CFUNCTION_CALL_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    using namespace VisualLinkerScript;

    /// @brief Represents a function call
    /// @brief: Example: ALIGN(4)
    class CFunctionCall : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_procedureName;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        SharedPtrVector<CLinkerScriptContentBase> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CFunctionCall(CRawEntry functionName,
                               CRawEntry openingParenthesis,
                               CRawEntry closingParenthesis,
                               SharedPtrVector<CLinkerScriptContentBase>&& parsedContent,
                               std::vector<CRawEntry>&& rawElements,
                               SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_procedureName(functionName),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(std::move(parsedContent))
        {}

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProcedureCall;
        }

        /// @brief Reports back the list of parameters
        const  std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the procedure-name
        const CRawEntry& ProcedureName()
        {
            return this->m_procedureName;
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
    };
}

#endif
