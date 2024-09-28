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
        explicit CSectionOutputType(const CRawEntry& typeEntry,
                                    const CRawEntry& openingParenthesis,
                                    const CRawEntry& closingParenthesis,
                                    const std::vector<CRawEntry>& rawElements,
                                    const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
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
