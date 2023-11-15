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
        explicit CSectionOutputDataExpression(CRawEntry dataExpressionEntry)
            : CLinkerScriptContentBase({ dataExpressionEntry }, {})
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputDataExpression;
        }

        /// @brief Reports back the PHDR regions name.
        const CRawEntry& PhdrRegion()
        {
            return this->m_phdrName;
        }

        /// @brief Reports back the colon initiating the statement.
        const CRawEntry& ColonEntry()
        {
            return this->m_colonEntry;
        }
    };
}

#endif
