#ifndef CGENERIC_SECTION_DEFINITION_H_
#define CGENERIC_SECTION_DEFINITION_H_

#include "CSectionDefinitionBase.h"
#include "CSectionOutput.h"
#include "CProgramHeaderButton.h"
#include "CFillExpressionButton.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents a Memory-Statement.
    class CSectionStatement : public CSectionDefinitionBase
    {
        DECLARE_READONLY_PROPERTY(SharedPtrVector<CSectionOutput>, ChildOutputs)

        /// @brief Default constructor
        CSectionStatement(
				const std::string& title, 
				const CFillExpressionButton& fillExpression, 
	            const std::vector<CProgramHeaderButton>& programHeaders, 
	            const SharedPtrVector<CSectionOutput>& childOutputs, 
				const bool inModelStartPosition,
				const bool inModelLength,
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown) :
    		CSectionDefinitionBase(
                title, 
                fillExpression, 
                programHeaders, 
                inModelStartPosition, 
                inModelLength, 
                startAddressKnown, 
                endAddressKnown, 
                memorySizeKnown),
    		m_ChildOutputs(childOutputs)
        {}

        /// @copydoc CDrawableObjectBase::Paint
        void Paint(
            const CGraphicContext& graphicContext,
            QPainter& painter) override;

        /// @copydoc CAddressedRegion::CalculateBodySize
        SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;

        /// @copydoc CAddressedRegion::SetBodyPosition
        void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;
    };
}

#endif // CGENERIC_SECTION_DEFINITION_H_
