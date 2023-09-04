#ifndef CSECTION_OUTPUT_FILL_EXPRESSION_H__
#define CSECTION_OUTPUT_FILL_EXPRESSION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputFillExpression : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_equalSign;
        CRawEntry m_fillExpressionValue;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputFillExpression(CRawEntry equalSign,
                                              CRawEntry fillExpressionValue,
                                              std::vector<CRawEntry>&& rawElements,
                                              SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_equalSign(equalSign),
              m_fillExpressionValue(fillExpressionValue)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputFillExpression;
        }

        /// @brief Reports back the PHDR Region
        const CRawEntry& EqualSign()
        {
            return this->m_equalSign;
        }

        /// @brief Reports back the FillExpressionValue Region
        const CRawEntry& FillExpressionValue()
        {
            return this->m_fillExpressionValue;
        }
    };
}

#endif
