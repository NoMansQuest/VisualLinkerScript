#ifndef CSECTION_STATEMENT_H__
#define CSECTION_STATEMENT_H__

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
	            const SharedPtrVector<CProgramHeaderButton>& programHeaders, 
	            const SharedPtrVector<CSectionOutput>& childOutputs, 
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
				const std::string& startAddress,
				const std::string& endAddress,
				const std::string& memorySize) :
    		CSectionDefinitionBase(
                title, 
                fillExpression, 
                programHeaders, 
                inModelStartPosition, 
                inModelLength, 
                startAddress, 
                endAddress, 
                memorySize),
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

    protected:
        /// @brief 5mm spacing (on the right side)
        int SizeMarkerDepth() override { return 5; }
    };
}

#endif // CSECTION_STATEMENT_H__
