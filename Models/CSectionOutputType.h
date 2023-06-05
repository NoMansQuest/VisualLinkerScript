#ifndef CSECTION_OUTPUT_TYPE_H__
#define CSECTION_OUTPUT_TYPE_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a procedural call
    class CSectionOutputType : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_typeEntry;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputType(CRawEntry typeEntry,
                                    CRawEntry openingParenthesis,
                                    CRawEntry closingParenthesis,
                                    std::vector<CRawEntry>&& rawElements,
                                    std::vector<CParserViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_typeEntry(typeEntry),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputType;
        }

        /// @brief Reports back the procedure-name
        const CRawEntry& TypeEntry()
        {
            return this->m_typeEntry;
        }

        /// @brief Reports back the openning parenthesis
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
