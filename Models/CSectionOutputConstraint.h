#ifndef CSECTIONOUTPUTCONSTRAINT_H
#define CSECTIONOUTPUTCONSTRAINT_H

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents an section- output constraint: ONLY_IF_RW and ONLY_IF_RO
    class CSectionOutputConstraint : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_sectionOutputConstraint;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputConstraint(const CRawEntry& sectionOutputConstraint)
            : CLinkerScriptContentBase({ sectionOutputConstraint }, {})
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionOutputConstraint;
        }

        /// @brief Reports back the PHDR regions name.
        const CRawEntry& Constraint()
        {
            return this->m_sectionOutputConstraint;
        }
    };
}

#endif // CSECTIONOUTPUTCONSTRAINT_H
