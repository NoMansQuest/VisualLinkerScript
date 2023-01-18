#ifndef CFUNCTION_CALL_STATEMENT_H__
#define CFUNCTION_CALL_STATEMENT_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "CExpression.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a procedural call
    class CFunctionCall : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_procedureName;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        std::vector<CLinkerScriptContentBase> m_parameters;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CFunctionCall(CRawEntry functionName,
                               CRawEntry openingParenthesis,
                               CRawEntry closingParenthesis,
                               std::vector<CLinkerScriptContentBase>&& parameters,
                               std::vector<CRawEntry>&& rawElements,
                               std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_procedureName(functionName),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parameters(std::move(parameters))
        {}

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProcedureCall;
        }

        /// @brief Reports back the list of parameters
        const std::vector<CLinkerScriptContentBase> Parameters()
        {
            return this->m_parameters;
        }

        /// @brief Reports back the procedure-name
        const CRawEntry& ProcedureName()
        {
            return this->m_procedureName;
        }

        /// @brief Reports back the openning parenthesis
        const CRawEntry& OpenningParenthesis()
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
