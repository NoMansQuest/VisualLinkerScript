#ifndef CSECTION_DEFINITION_BASE_H__
#define CSECTION_DEFINITION_BASE_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CAddressedRegion.h"
#include "CFillExpressionButton.h"
#include "CProgramHeaderButton.h"
#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents a Memory-Statement.
    class CSectionDefinitionBase : public CAddressedRegion
    {
        DECLARE_READONLY_PROPERTY(std::string, Title)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, HeaderArea)
	    DECLARE_STANDARD_PROPERTY(CFillExpressionButton, FillExpression)
        DECLARE_STANDARD_PROPERTY(std::vector<CProgramHeaderButton>, ProgramHeaders)

        /// @brief Default constructor
        CSectionDefinitionBase(
	            std::string title,
	            CFillExpressionButton fillExpression, 
	            const std::vector<CProgramHeaderButton>& programHeaders,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown) :			
			 CAddressedRegion(inModelStartPosition, inModelLength, startAddressKnown, endAddressKnown, memorySizeKnown),
		     m_Title(std::move(title)),
		     m_FillExpression(std::move(fillExpression)),
		     m_ProgramHeaders(programHeaders)
	    {}

        /// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;

        /// @copydoc CAddressedRegion::SetBodyPosition
        void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;
    };
}

#endif // CMEMORYSTATEMENT_H