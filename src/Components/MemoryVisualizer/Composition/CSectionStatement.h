#ifndef CSECTION_STATEMENT_H__
#define CSECTION_STATEMENT_H__

#include "Components/MemoryVisualizer/Composition/CSectionDefinitionBase.h"
#include "Components/MemoryVisualizer/Composition/CSectionOutput.h"
#include "Components/MemoryVisualizer/Composition/CProgramHeaderButton.h"
#include "Components/MemoryVisualizer/Composition/CFillExpressionButton.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents a Memory-Statement.
    class CSectionStatement : public CSectionDefinitionBase
    {
        DECLARE_READONLY_PROPERTY(std::shared_ptr<LinqVector<CSectionOutput>>, ChildOutputs)

        /// @brief Default constructor
        CSectionStatement(
				const std::string& title, 
				const CFillExpressionButton& fillExpression, 
	            const std::shared_ptr<LinqVector<CProgramHeaderButton>>& programHeaders,
	            const std::shared_ptr<LinqVector<CSectionOutput>>& childOutputs,
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
