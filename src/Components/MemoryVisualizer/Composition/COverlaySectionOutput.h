#ifndef COVERLAY_SECTION_OUTPUT_H__
#define COVERLAY_SECTION_OUTPUT_H__

#include "Components/MemoryVisualizer/Composition/CAddressedRegion.h"
#include "Helpers.h"
#include "Components/MemoryVisualizer/Composition/SMetricRectangleF.h"
#include "Components/MemoryVisualizer/Composition/SMetricSizeF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	class CGraphicContext;

	/// @brief Represents an 'Overlay' section found in "OVERLAY" statements.
	class COverlaySectionOutput : public CAddressedRegion
	{
		DECLARE_READONLY_PROPERTY(std::string, Content)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, ContentArea)

		/// @brief Default constructor.
		COverlaySectionOutput(std::string content,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength) :
			CAddressedRegion(inModelStartPosition, inModelLength, "", "", ""),
			m_Content(std::move(content))
		{}

		/// @copydoc CDrawableObjectBase::Paint
		void Paint(const CGraphicContext& graphicContext, QPainter& painter) override;

		/// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;

		/// @copydoc CAddressedRegion::SetBodyPosition
		void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;

	protected:
		/// @brief Not used for this object
		int SizeMarkerDepth() override { return 0; }
	};
}

#endif // COVERLAY_SECTION_OUTPUT_H__
