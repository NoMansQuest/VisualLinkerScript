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
				SharedPtrVector<COverlaySectionStatement> childSections,
				const CFillExpressionButton& fillExpression,
	            const std::vector<CProgramHeaderButton>& programHeaders,
	            const uint32_t inModelStartPosition,
	            const uint32_t inModelLength,
	            const bool startAddressKnown,
	            const bool endAddressKnown,
	            const bool memorySizeKnown) :
            CSectionDefinitionBase(
                "",
                fillExpression,
                programHeaders,
                inModelStartPosition,
                inModelLength,
                startAddressKnown,
                endAddressKnown,
                memorySizeKnown),
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
    };
}

#endif // COVERLAY_STATEMENT_H__
