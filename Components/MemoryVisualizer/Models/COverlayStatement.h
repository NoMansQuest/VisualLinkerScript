#ifndef COVERLAY_STATEMENT_H__
#define COVERLAY_STATEMENT_H__

#include <vector>
#include <string>
#include <cstdint>
#include "COverlaySectionStatement.h"
#include "../../../Helpers.h"
#include "CProgramHeaderButton.h"
#include "CFillExpressionButton.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
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

        /// @copydoc CAddressedRegion::CalculateDesiredSize
        SMetricSizeF CalculateDesiredSize(
            const QFontMetrics& fontMetricsSmall,
            const QFontMetrics& fontMetricsLarge) override;

        /// @copydoc CAddressedRegion::SetGeometry
        void SetGeometry(SMetricRectangleF allocatedArea) override;
    };
}

#endif // COVERLAY_STATEMENT_H__
