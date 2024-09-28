#ifndef CSECTION_OUTPUT_SPECIAL_KEYWORD_H__
#define CSECTION_OUTPUT_SPECIAL_KEYWORD_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an "AtLmaRegion" assigned to a "Section Output"
    class CSectionOutputDataExpression : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_dataExpression;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputDataExpression(const CRawEntry& dataExpressionEntry)
            : CLinkerScriptContentBase({ dataExpressionEntry }, {})
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputDataExpression;
        }

        /// @brief Reports back the PHDR regions name.
        const CRawEntry& DataExpression()
        {
            return this->m_dataExpression;
        }
    };
}

#endif
