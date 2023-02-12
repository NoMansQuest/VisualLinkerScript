#ifndef CSECTION_OUTPUT_TYPE_H__
#define CSECTION_OUTPUT_TYPE_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a procedural call
    class CSectionOutputType : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_typeEntry;
        CRawEntry m_openingParenthesis;
        CRawEntry m_closingParenthesis;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputType(CRawEntry typeEntry,
                                    CRawEntry openingParenthesis,
                                    CRawEntry closingParenthesis,
                                    std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                    std::vector<CRawEntry>&& rawElements,
                                    std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_typeEntry(typeEntry),
              m_openingParenthesis(openingParenthesis),
              m_closingParenthesis(closingParenthesis),
              m_parsedContent(std::move(parsedContent))
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputType;
        }

        /// @brief Reports back parsed content.
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the procedure-name
        const CRawEntry& TypeEntry()
        {
            return this->m_typeEntry;
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
