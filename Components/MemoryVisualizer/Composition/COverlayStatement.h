#ifndef COVERLAY_STATEMENT_H__
#define COVERLAY_STATEMENT_H__

#include <vector>
#include <string>
#include <cstdint>
#include "COverlaySectionStatement.h"
#include "../../../Helpers.h"
#include "CProgramHeaderButton.h"
#include "CFillExpressionButton.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents an 'Overlay' object found in "SECTION"
    class COverlayStatement : public CSectionDefinitionBase
    {       
        DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionStatement>, OverlaySections)
        DECLARE_READONLY_PROPERTY(std::string, LoadRegion)

        /// @brief Default constructor.    	
        COverlayStatement(
				const CFillExpressionButton& fillExpression,
				SharedPtrVector<CProgramHeaderButton> programHeaders,
				SharedPtrVector<COverlaySectionStatement> childSections,
	            const uint32_t inModelStartPosition,
	            const uint32_t inModelLength,
	            const std::string& startAddress,
	            const std::string& endAddress,
	            const std::string& memorySize) :
            CSectionDefinitionBase(
                "",
                fillExpression,
                programHeaders,
                inModelStartPosition,
                inModelLength,
                startAddress,
                endAddress,
                memorySize),
            m_OverlaySections(std::move(childSections))
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
        /// @brief Not used for this object
        int SizeMarkerDepth() override { return 0; }
    };
}

#endif // COVERLAY_STATEMENT_H__
